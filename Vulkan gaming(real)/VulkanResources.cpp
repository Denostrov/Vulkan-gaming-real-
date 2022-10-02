#include "VulkanResources.h"

using namespace std::literals;

VulkanResources::VulkanResources()
{
	glfwInit();

	renderWindow = std::make_unique<Window>(800, 600);

	auto extensionProperties = vk::enumerateInstanceExtensionProperties();
	printConsole("Instance extensions:\n"sv);
	for (auto const& property : extensionProperties)
	{
		printConsole("\tExtension name: {}, Version: {}\n"sv, property.extensionName, property.specVersion);
	}
}

VulkanResources::~VulkanResources()
{
	glfwTerminate();
}

bool VulkanResources::windowCloseStatus()
{
	return glfwWindowShouldClose(*renderWindow.get());
}
