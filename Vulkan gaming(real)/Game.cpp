#include "Game.h"

#include "EventHandler.h"

Game::Game()
	:eventHandler(), debugFont{512, 512, 1.0f / 22.0f, 27, 20, 40}, debugTextBox({0.0f, -1.0f, 0.0f}, {1.0f, 0.5f}, debugFont), mineMap{30, 15, debugFont}
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
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		auto [xPos, yPos] = vulkan->getCursorCoordinates();
		mineMap.onMousePressed(xPos, yPos);
		break;
	}
	default:
		break;
	}
}

void Game::onMouseButtonHeld(int button)
{

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

void Game::onKeyHeld(int key)
{

}

void Game::update()
{
	processInput();
	debugTextBox.update();
}

void Game::processInput()
{
	vulkan->framebufferResized = eventHandler.getFramebufferResized();
	for (auto key : eventHandler.getPressedKeys()) onKeyPressed(key);
	for (auto key : eventHandler.getHeldKeys()) onKeyHeld(key);
	for (auto button : eventHandler.getPressedMouseButtons()) onMouseButtonPressed(button);
	for (auto button : eventHandler.getHeldMouseButtons()) onMouseButtonHeld(button);
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
