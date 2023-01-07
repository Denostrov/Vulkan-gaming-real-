#include "Game.h"

#include "EventHandler.h"

Game::Game()
	:eventHandler(), debugFont{"textures/DejaVu mono.json"}, debugTextBox({0.0f, -1.0f, 0.0f}, {1.0f, 0.5f}, debugFont), gameOverFlash(debugFont),
	mineMap{ 30, 15, 50, debugFont, {observer} }, resetButton({ -2.0f / 16.0f, -1.0f, -0.1f }, { 4.0f / 16.0f, 2.0f / 16.0f }, debugFont, "lmao"s,
		MemberFunction(mineMap, &Map::reset)), remainingMines("Mines: "s + std::to_string(mineMap.getMineCount()), debugFont, {-1.0f, -0.925f, -0.1f}),
	gameTimerText("0", debugFont, {0.75f, -0.925f, -0.1f})
{
	vulkan = std::make_unique<VulkanResources>(&eventHandler);

	startLoop();
}

bool Game::gameShouldStop()
{
	return vulkan->windowCloseStatus();
}

void Game::startLoop()
{
	double currentTime = glfwGetTime();
	double elapsedTime = 0.0;
	double FPSTime = 0.0;
	double deltaTime = 0.0;
	double newTime = currentTime;

	while (!gameShouldStop())
	{
		newTime = glfwGetTime();
		deltaTime = newTime - currentTime;
		elapsedTime += deltaTime;
		FPSTime += deltaTime;
		currentTime = newTime;
		uint64_t updateCount = 0;

		if (elapsedTime > TIME_STEP)
		{
			eventHandler.pollEvents();
			if (FPSTime > 1.0)
			{
				if (showFPSCounter)
				{
					updateFPSCounter();
				}

				FPSTime = std::fmod(FPSTime, 1.0);
				FPSCount = 0;
			}

			while (elapsedTime > TIME_STEP && updateCount < 4)
			{
				elapsedTime -= TIME_STEP;
				update();
				updateCount++;
			}
			if (updateCount >= 4)
			{
				elapsedTime = 0.0;
			}

			updateCount = 0;
		}

		vulkan->drawFrame();
		FPSCount++;
	}

	vulkan->stopRendering();
}

void Game::onMouseButtonPressed(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_RIGHT:
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		auto [xPos, yPos] = vulkan->getCursorCoordinates();
		mineMap.onMousePressed(xPos, yPos, button == GLFW_MOUSE_BUTTON_LEFT);
		if (button == GLFW_MOUSE_BUTTON_LEFT) resetButton.onMousePressed(xPos, yPos);
		remainingMines.setText("Mines: " + std::to_string(mineMap.getMineCount() - mineMap.getMarkedCellCount()));
		break;
	}
	default:
		break;
	}
}

void Game::onMouseButtonHeld(int)
{

}

void Game::onMouseButtonReleased(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_RIGHT:
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		mineMap.onMouseReleased();
		break;
	}
	default:
		break;
	}
}

void Game::onKeyPressed(int key)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		vulkan->setWindowShouldClose();
		break;
	case GLFW_KEY_F2:
		toggleFPSCounter();
		debugTextBox.addText("Toggled FPS counter"s, 512ULL);
		break;
	case GLFW_KEY_F3:
		vulkan->toggleWireframeMode();
		debugTextBox.addText("Toggled wireframe mode"s, 512ULL);
		break;
	case GLFW_KEY_F4:
	{
		uint64_t temp = 0;
		ObjectPools::quads.add(QuadComponent({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}), &temp);
		break;
	}
	default:
		break;
	}
}

void Game::onKeyHeld(int)
{

}

void Game::onMapStateChanged(Map::State newState)
{
	switch (newState)
	{
	case Map::State::ePreparing:
		resetButton.changeText("lmao"s);
		gameTimer = 0;
		gameTimerText.setText("0");
		remainingMines.setText("Mines: "s + std::to_string(mineMap.getMineCount()));
		break;
	case Map::State::eLost:
		resetButton.changeText("retard"s);
		gameOverFlash.start({ 1.0f, 0.0f, 0.0f }, 0.1);
		break;
	case Map::State::eWon:
		resetButton.changeText("based"s);
		gameOverFlash.start({ 0.0f, 1.0f, 0.0f }, 0.1);
		break;
	case Map::State::ePlaying:
		resetButton.changeText("yooo"s);
		break;
	default:
		break;
	}
}

void Game::update()
{
	processInput();
	debugTextBox.update();
	gameOverFlash.update();
	for (auto&& notification : observer.getNotifications())
	{
		if (notification.first == NotifierType::eMap)
		{
			onMapStateChanged((Map::State)notification.second);
		}
	}
	if (mineMap.getCurrentState() == Map::State::ePlaying && mineMap.getCoveredCellCount() != mineMap.getCellCount() - mineMap.getMineCount())
	{
		gameTimer += TIME_STEP;
		gameTimerText.setText(std::to_string(std::roundf((float)gameTimer * 100.0f) / 100.0f));
	}
}

void Game::processInput()
{
	vulkan->framebufferResized = eventHandler.getFramebufferResized();
	for (auto key : eventHandler.getPressedKeys()) onKeyPressed(key);
	for (auto key : eventHandler.getHeldKeys()) onKeyHeld(key);
	for (auto button : eventHandler.getPressedMouseButtons()) onMouseButtonPressed(button);
	for (auto button : eventHandler.getHeldMouseButtons()) onMouseButtonHeld(button);
	for (auto button : eventHandler.getReleasedMouseButtons()) onMouseButtonReleased(button);
}

void Game::updateFPSCounter()
{
	FPSCounter = std::make_unique<Text>("FPS:"s + std::to_string(FPSCount), debugFont, glm::vec3(-1.0f, -1.0f, 0.0f));
}

void Game::toggleFPSCounter()
{
	if (!showFPSCounter)
	{
		showFPSCounter = true;
		updateFPSCounter();
	}
	else
	{
		showFPSCounter = false;
		FPSCounter.reset(nullptr);
	}
}
