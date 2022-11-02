#pragma once

#include <memory>

#include "VulkanResources.h"
#include "EventHandler.h"

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

	std::unique_ptr<VulkanResources> vulkan;

	EventHandler eventHandler;
	friend class EventHandler;
};

