#pragma once

#include <memory>

#include "VulkanResources.h"
#include "EventHandler.h"
#include "Text.h"
#include "Map.h"

class EventHandler;

class Game
{
public:
	Game();

private:
	bool gameShouldStop();
	void startLoop();

	void onKeyPressed(int key);
	void onKeyHeld(int key);

	void update();

	bool showFPSCounter = false;
	uint64_t FPSCount = 0;
	std::unique_ptr<Text> FPSCounter;
	void updateFPSCounter();
	void toggleFPSCounter();

	Font debugFont;
	TextBox debugTextBox;

	std::unique_ptr<VulkanResources> vulkan;

	EventHandler eventHandler;
	friend class EventHandler;

	Map mineMap;
};

