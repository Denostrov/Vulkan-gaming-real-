#include "Window.h"

#include "constants.h"
#include "VulkanResources.h"
#include "helpers.h"

void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto vulkan = reinterpret_cast<VulkanResources*>(glfwGetWindowUserPointer(window));
	vulkan->framebufferResized = true;
}

WindowContext::WindowContext()
{
	glfwInit();
}

WindowContext::~WindowContext()
{
	glfwTerminate();
}

Window::Window(int32_t width, int32_t height, WindowContext const&, VulkanResources* vulkan)
	:width(width), height(height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	assert(width > 0 && height > 0);
	window = glfwCreateWindow(width, height, "Cocksweeper", nullptr, nullptr);
	glfwSetWindowUserPointer(window, vulkan);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

Window::operator GLFWwindow* () const
{
	return window;
}
