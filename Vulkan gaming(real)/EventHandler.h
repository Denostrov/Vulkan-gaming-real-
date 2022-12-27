#pragma once

#include <unordered_set>

#include "constants.h"

class Game;

enum class EventType
{
	FramebufferResize, Key
};

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void framebufferResizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

class EventHandler
{
public:
	explicit EventHandler();

	void pollEvents();

	std::unordered_set<int> getPressedKeys();
	std::unordered_set<int> getHeldKeys();

	std::unordered_set<int> getPressedMouseButtons();
	std::unordered_set<int> getHeldMouseButtons();
	std::unordered_set<int> getReleasedMouseButtons();

	bool getFramebufferResized();

private:
	void onMouseButtonEvent(int button, int action, int mods);
	void onKeyEvent(int key, int scancode, int action, int mods);
	void onFramebufferResizeEvent();

	std::unordered_set<int> keysPressed;
	std::unordered_set<int> keysHeld;

	std::unordered_set<int> mouseButtonsPressed;
	std::unordered_set<int> mouseButtonsHeld;
	std::unordered_set<int> mouseButtonsReleased;

	bool framebufferResized = false;

	friend void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	friend void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};