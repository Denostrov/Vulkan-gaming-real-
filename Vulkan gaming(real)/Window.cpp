#include "Window.h"

WindowContext::WindowContext()
{
	glfwInit();
}

WindowContext::~WindowContext()
{
	glfwTerminate();
}

Window::Window(int32_t width, int32_t height, WindowContext const&)
	:width(width), height(height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	assert(width > 0 && height > 0);
	window = glfwCreateWindow(width, height, "Cocksweeper", nullptr, nullptr);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

Window::operator GLFWwindow* () const
{
	return window;
}
