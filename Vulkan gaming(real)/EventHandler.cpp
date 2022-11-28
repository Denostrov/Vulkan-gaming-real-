#include "EventHandler.h"

#include "Game.h"

EventHandler::EventHandler()
{}

void EventHandler::pollEvents()
{
	glfwPollEvents();
}

std::unordered_set<int> EventHandler::getPressedKeys()
{
	std::unordered_set<int> result = keysPressed;
	keysPressed.clear();
	return result;
}

std::unordered_set<int> EventHandler::getHeldKeys()
{
	return keysHeld;
}

std::unordered_set<int> EventHandler::getPressedMouseButtons()
{
	std::unordered_set<int> result = mouseButtonsPressed;
	mouseButtonsPressed.clear();
	return result;
}

std::unordered_set<int> EventHandler::getHeldMouseButtons()
{
	return mouseButtonsHeld;
}

bool EventHandler::getFramebufferResized()
{
	auto result = framebufferResized;
	framebufferResized = false;
	return result;
}

void EventHandler::onMouseButtonEvent(int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		mouseButtonsPressed.insert(button);
		mouseButtonsHeld.insert(button);
	}
	else if (action == GLFW_RELEASE)
	{
		mouseButtonsHeld.erase(button);
	}
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
	framebufferResized = true;
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

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto eventHandler = reinterpret_cast<EventHandler*>(glfwGetWindowUserPointer(window));
	eventHandler->onMouseButtonEvent(button, action, mods);
}