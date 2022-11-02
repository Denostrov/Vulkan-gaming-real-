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
	while (!gameShouldStop())
	{
		eventHandler.pollEvents();
		vulkan->drawFrame();
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
	case GLFW_KEY_F3:
		vulkan->toggleWireframeMode();
		break;
	case GLFW_KEY_F4:
	{
		uint64_t temp = 0;
		vulkan->addQuad(QuadComponent({0.0f, 0.0f, 0.0f}, 1.0f, {0.0f, 0.0f}, {0.5f, 0.5f}), &temp);
		break;
	}
	default:
		break;
	}
}

void Game::onKeyHeld(int key)
{
	switch (key)
	{
	default:
		break;
	}
}