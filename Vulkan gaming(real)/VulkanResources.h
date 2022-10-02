#pragma once

#include "print.h"
#include "Window.h"

class VulkanResources
{
public:
	VulkanResources();
	~VulkanResources();

	bool windowCloseStatus();
private:
	std::unique_ptr<Window> renderWindow;
};

