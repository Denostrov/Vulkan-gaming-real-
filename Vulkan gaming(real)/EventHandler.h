#pragma once

#include <unordered_set>

#include "constants.h"

class Game;

enum class EventType
{
	FramebufferResize, Key
};

void framebufferResizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

class EventHandler
{
public:
	explicit EventHandler(Game& game);

	void pollEvents();

private:
	void onKeyEvent(int key, int scancode, int action, int mods);
	void onFramebufferResizeEvent();
	std::vector<int> getPressedKeys();
	std::vector<int> getHeldKeys();

	std::unordered_set<int> keysPressed;
	std::unordered_set<int> keysHeld;

	Game& game;

	friend void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};