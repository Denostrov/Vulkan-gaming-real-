#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int32_t width, int32_t height);
	~Window();

	Window(Window const& window) = delete;
	Window& operator=(Window const& rhs) = delete;
	Window(Window&& window) = default;
	Window& operator=(Window&& rhs) = default;

	operator GLFWwindow* () const;

private:
	int32_t width, height;
	GLFWwindow* window;
};

