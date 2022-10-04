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
	auto layerProperties = vk::enumerateInstanceLayerProperties();
	printConsole("{} available layers:\n"sv, layerProperties.size());
	for (auto const& property : layerProperties)
	{
		printLabelValuePairs(std::cout,
			LabelValuePair{"Layer name"s, property.layerName},
			LabelValuePair{"Version"s, property.implementationVersion},
			LabelValuePair{"Description"s, property.description});
	}

	auto getRequiredLayers = [&]()
	{
		printConsole("{} required layers:\n"sv, VALIDATION_LAYERS.size());
		for (auto const& layer : VALIDATION_LAYERS)
		{
			bool isSupported = checkVectorContainsString(layerProperties, [](vk::LayerProperties const& p) { return p.layerName; }, layer);
			printConsole("\t{}\t"s + (isSupported ? "(supported)"s : "(not supported)"s) + "\n"s, layer);
			assert(isSupported);
		}
		return std::vector<char const*>(VALIDATION_LAYERS.begin(), VALIDATION_LAYERS.end());
	};

	auto getRequiredInstanceExtensions = [&]()
	{
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		auto glfwExtensionsVector = std::vector<char const*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

		printConsole("{} required instance extensions:\n"sv, glfwExtensionsVector.size());
		for (auto const& extension : glfwExtensionsVector)
		{
			bool isSupported = checkVectorContainsString(extensionProperties, [](vk::ExtensionProperties const& p) { return p.extensionName; }, extension);
			printConsole("\t{}\t"s + (isSupported ? "(supported)"s : "(not supported)"s) + "\n"s, extension);
			assert(isSupported);
		}
		return glfwExtensionsVector;
	};

	vk::ApplicationInfo applicationInfo{"Copesweeper", VK_MAKE_API_VERSION(0, 1, 0, 0), "Vulkan engine", VK_MAKE_API_VERSION(0, 1, 0, 0), VK_API_VERSION_1_3};
	auto validationLayers = ENABLE_VALIDATION_LAYERS ? getRequiredLayers() : std::vector<char const*>();
	auto glfwExtensions = getRequiredInstanceExtensions();
	vk::InstanceCreateInfo instanceCreateInfo{{}, &applicationInfo, validationLayers, glfwExtensions};
	instance = vk::createInstanceUnique(instanceCreateInfo);
	assert(instance);
}

VulkanResources::~VulkanResources()
{}

bool VulkanResources::windowCloseStatus()
{
	return glfwWindowShouldClose(renderWindow);
}
