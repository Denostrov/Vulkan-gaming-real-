#pragma once

#include "print.h"
#include "helpers.h"
#include "constants.h"
#include "Window.h"
#include "logging.h"

class VulkanResources
{
public:
	VulkanResources();
	~VulkanResources();

	bool windowCloseStatus();
private:
	WindowContext windowContext;
	Window renderWindow;
	vk::UniqueInstance instance;
	vk::UniqueDebugUtilsMessengerEXT debugMessenger;
	vk::UniqueDevice device;
	
};

