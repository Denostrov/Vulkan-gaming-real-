#include "Game.h"

Game::Game()
{
	vulkan = std::make_unique<VulkanResources>();

	startLoop();
}

bool Game::gameShouldStop()
{
	return vulkan->windowCloseStatus();
}

void Game::startLoop()
{
	while (!gameShouldStop())
	{
		glfwPollEvents();
		vulkan->drawFrame();
	}

	vulkan->stopRendering();
}
