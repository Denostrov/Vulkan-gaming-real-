#pragma once

#include "constants.h"

class VulkanResources;

struct WindowContext
{
	WindowContext();
	~WindowContext();

	WindowContext(WindowContext const&) = delete;
	WindowContext& operator=(WindowContext const&) = delete;
};

class Window
{
public:
	Window(int32_t width, int32_t height, WindowContext const&, VulkanResources* vulkan);
	~Window();

	Window(Window const&) = delete;
	Window& operator=(Window const&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

	operator GLFWwindow* () const;

private:
	int32_t width, height;
	GLFWwindow* window;
};

