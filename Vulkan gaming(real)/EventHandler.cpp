#include "EventHandler.h"

#include "Game.h"

EventHandler::EventHandler(Game& game)
	:game(game)
{}

void EventHandler::pollEvents()
{
	glfwPollEvents();
	for (auto key : getPressedKeys())
	{
		game.onKeyPressed(key);
	}
	for (auto key : getHeldKeys())
	{
		game.onKeyHeld(key);
	}
	if (glfwGetMouseButton(game.vulkan->renderWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double xPos, yPos;
		int windowWidth, windowHeight;
		glfwGetCursorPos(game.vulkan->renderWindow, &xPos, &yPos);
		glfwGetWindowSize(game.vulkan->renderWindow, &windowWidth, &windowHeight);
		xPos = xPos / windowWidth * 2.0 - 1.0;
		yPos = yPos / windowHeight * 2.0 - 1.0;
		game.mineMap.onMousePressed(xPos, yPos);
	}
}

std::vector<int> EventHandler::getPressedKeys()
{
	std::vector<int> result;
	for (auto key : keysPressed)
	{
		result.push_back(key);
	}
	keysPressed.clear();
	return result;
}

std::vector<int> EventHandler::getHeldKeys()
{
	std::vector<int> result;
	for (auto key : keysHeld)
	{
		result.push_back(key);
	}
	return result;
}

void EventHandler::onKeyEvent(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keysPressed.insert(key);
		keysHeld.insert(key);
	}
	else if (action == GLFW_RELEASE)
	{
		keysHeld.erase(key);
	}
}

void EventHandler::onFramebufferResizeEvent()
{
	game.vulkan->framebufferResized = true;
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto eventHandler = reinterpret_cast<EventHandler*>(glfwGetWindowUserPointer(window));
	eventHandler->onFramebufferResizeEvent();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto eventHandler = reinterpret_cast<EventHandler*>(glfwGetWindowUserPointer(window));
	eventHandler->onKeyEvent(key, scancode, action, mods);
}