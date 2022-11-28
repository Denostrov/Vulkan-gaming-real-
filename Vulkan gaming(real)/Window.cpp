#include "Window.h"

#include "constants.h"
#include "Game.h"
#include "helpers.h"

WindowContext::WindowContext()
{
	glfwInit();
}

WindowContext::~WindowContext()
{
	glfwTerminate();
}

Window::Window(int32_t width, int32_t height, WindowContext const&, EventHandler* eventHandler)
	:width(width), height(height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	assert(width > 0 && height > 0);
	window = glfwCreateWindow(width, height, "Cocksweeper", nullptr, nullptr);
	glfwSetWindowUserPointer(window, eventHandler);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

Window::operator GLFWwindow* () const
{
	return window;
}
