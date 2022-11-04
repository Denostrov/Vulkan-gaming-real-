#include "Game.h"

#include "EventHandler.h"

Game::Game()
	:eventHandler(*this)
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
					FPSCounter = std::make_unique<Text>("FPS:"s + std::to_string(FPSCount), glm::vec3(-1.0f, -1.0f, 0.0f));
				}

				FPSTime = std::fmod(FPSTime, 1.0);
				FPSCount = 0;
			}

			while (elapsedTime > TIME_STEP && updateCount < 4)
			{
				elapsedTime -= TIME_STEP;
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

void Game::onKeyPressed(int key)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		vulkan->setWindowShouldClose();
		break;
	case GLFW_KEY_F2:
		if (!showFPSCounter)
		{
			showFPSCounter = true;
			FPSCounter = std::make_unique<Text>("FPS:"s + std::to_string(FPSCount), glm::vec3(-1.0f, -1.0f, 0.0f));
		}
		else
		{
			showFPSCounter = false;
			FPSCounter.reset(nullptr);
		}
		break;
	case GLFW_KEY_F3:
		vulkan->toggleWireframeMode();
		break;
	case GLFW_KEY_F4:
	{
		uint64_t temp = 0;
		ObjectPools::quads.add(QuadComponent({0.0f, 0.0f, 0.0f}, 1.0f, {0.0f, 0.0f}, {0.5f, 0.5f}), &temp);
		break;
	}
	default:
		break;
	}
}

void Game::onKeyHeld(int key)
{

}