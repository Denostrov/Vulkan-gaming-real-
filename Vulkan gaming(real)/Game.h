#pragma once

#include <memory>

#include "VulkanResources.h"
#include "EventHandler.h"
#include "Text.h"

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

	bool showFPSCounter = false;
	uint64_t FPSCount = 0;
	std::unique_ptr<Text> FPSCounter;

	std::unique_ptr<VulkanResources> vulkan;

	EventHandler eventHandler;
	friend class EventHandler;
};

