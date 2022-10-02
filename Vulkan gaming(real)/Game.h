#pragma once

#include <memory>

#include "VulkanResources.h"

class Game
{
public:
	Game();

private:
	bool gameShouldStop();
	void startLoop();

	std::unique_ptr<VulkanResources> vulkan;
};

