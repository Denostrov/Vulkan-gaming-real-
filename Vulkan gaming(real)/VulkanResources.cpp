#include "VulkanResources.h"

using namespace std::literals;

VulkanResources::VulkanResources()
	:windowContext(),
	renderWindow(800, 600, windowContext)
{
	auto extensionProperties = vk::enumerateInstanceExtensionProperties();
	printConsole("{} available instance extensions:\n"sv, extensionProperties.size());
	for (auto const& property : extensionProperties)
	{
		printLabelValuePairs(std::cout,
			LabelValuePair{"Extension name"s, property.extensionName},
			LabelValuePair{"Version"s, property.specVersion});
	}

	auto getRequiredLayers = []()
	{
		auto layerProperties = vk::enumerateInstanceLayerProperties();
		printConsole("{} available layers:\n"sv, layerProperties.size());
		for (auto const& property : layerProperties)
		{
			printLabelValuePairs(std::cout,
				LabelValuePair{"Layer name"s, property.layerName},
				LabelValuePair{"Version"s, property.implementationVersion},
				LabelValuePair{"Description"s, property.description});
		}


	};

	auto getRequiredInstanceExtensions = [&]()
	{
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		auto glfwExtensionsVector = std::vector<char const*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

		printConsole("{} required instance extensions:\n"sv, glfwExtensionsVector.size());
		for (auto const& extension : glfwExtensionsVector)
		{
			bool isSupported = std::find_if(extensionProperties.begin(), extensionProperties.end(),
				[&](vk::ExtensionProperties const& p) { return std::strcmp(p.extensionName, extension); }) != extensionProperties.end();
			printConsole("\t{}\t"s + (isSupported ? "(supported)"s : "(not supported)"s) + "\n"s, extension);
			assert(isSupported);
		}
		return glfwExtensionsVector;
	};

	vk::ApplicationInfo applicationInfo{"Copesweeper", VK_MAKE_API_VERSION(0, 1, 0, 0), "Vulkan engine", VK_MAKE_API_VERSION(0, 1, 0, 0), VK_API_VERSION_1_3};
	auto glfwExtensions = getRequiredInstanceExtensions();
	vk::InstanceCreateInfo instanceCreateInfo{{}, &applicationInfo, {}, glfwExtensions};
	instance = vk::createInstanceUnique(instanceCreateInfo);
	assert(instance);
}

VulkanResources::~VulkanResources()
{}

bool VulkanResources::windowCloseStatus()
{
	return glfwWindowShouldClose(renderWindow);
}
