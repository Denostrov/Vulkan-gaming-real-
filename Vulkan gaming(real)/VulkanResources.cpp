#include "VulkanResources.h"

using namespace std::literals;

template<class Resource>
auto enumerateFunc() {}
template<>
auto enumerateFunc<vk::ExtensionProperties>() { return vk::enumerateInstanceExtensionProperties(); }
template<>
auto enumerateFunc<vk::LayerProperties>() { return vk::enumerateInstanceLayerProperties(); }

template<class Resource>
auto enumerateRequiredFunc() {}
template<>
auto enumerateRequiredFunc<vk::ExtensionProperties>()
{
	uint32_t glfwExtensionCount = 0;
	auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	auto glfwExtensionsVector = std::vector<char const*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (ENABLE_VALIDATION_LAYERS)
	{
		glfwExtensionsVector.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return glfwExtensionsVector;
}
template<>
auto enumerateRequiredFunc<vk::LayerProperties>()
{
	return std::vector<char const*>(VALIDATION_LAYERS.begin(), VALIDATION_LAYERS.end());
}

template<class Resource>
auto getResourceNameFunc(Resource const& val) {}
template<>
auto getResourceNameFunc(vk::ExtensionProperties const& val)
{
	return val.extensionName;
}
template<>
auto getResourceNameFunc(vk::LayerProperties const& val)
{
	return val.layerName;
}

template<class Resource>
auto checkAndPrintRequired(std::vector<Resource> const& resources, std::vector<char const*> const& requiredResourceNames)
{
	std::cout << getTotalString(requiredResourceNames, "required"s, getFormatString(resources));
	for (auto name : requiredResourceNames)
	{
		bool isSupported = checkVectorContainsString(resources, getResourceNameFunc<Resource>, name);
		formatPrint(std::cout, "\t{}\t"s + (isSupported ? "(supported)"s : "(not supported)"s) + "\n"s, name);
		assert(isSupported);
	}
}

template<class Resource>
auto getRequiredResources()
{
	auto resources = enumerateFunc<Resource>();

	std::cout << getTotalString(resources, "available"s) << resources;

	auto requiredResources = enumerateRequiredFunc<Resource>();
	checkAndPrintRequired<Resource>(resources, requiredResources);

	return requiredResources;
}


VulkanResources::VulkanResources()
	:windowContext(),
	renderWindow(800, 600, windowContext)
{
	vk::DynamicLoader dynamicLoader;
	auto vkGetInstanceProcAddr = dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

	auto getDebugUtilsMessengerCreateInfo = []()
	{
		using enum vk::DebugUtilsMessageTypeFlagBitsEXT;
		using enum vk::DebugUtilsMessageSeverityFlagBitsEXT;
		vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo({}, eError | eWarning | eInfo | eVerbose,
																		   eGeneral | eValidation | ePerformance, debugCallback, nullptr);
		return debugUtilsMessengerCreateInfo;
	};

	vk::ApplicationInfo applicationInfo{"Copesweeper", VK_MAKE_API_VERSION(0, 1, 0, 0), "Vulkan engine", VK_MAKE_API_VERSION(0, 1, 0, 0), VK_API_VERSION_1_3};
	auto validationLayers = ENABLE_VALIDATION_LAYERS ? getRequiredResources<vk::LayerProperties>() : std::vector<char const*>();
	auto glfwExtensions = getRequiredResources<vk::ExtensionProperties>();
	auto debugUtilsMessengerCreateInfo = getDebugUtilsMessengerCreateInfo();
	vk::InstanceCreateInfo instanceCreateInfo{{}, &applicationInfo, validationLayers, glfwExtensions, ENABLE_VALIDATION_LAYERS ? &debugUtilsMessengerCreateInfo : nullptr};
	instance = vk::createInstanceUnique(instanceCreateInfo);
	assert(instance);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.get());

	if (ENABLE_VALIDATION_LAYERS)
	{
		debugMessenger = instance.get().createDebugUtilsMessengerEXTUnique(getDebugUtilsMessengerCreateInfo());
		assert(debugMessenger);
	}

	auto physicalDevices = instance->enumeratePhysicalDevices();
}

VulkanResources::~VulkanResources()
{}

bool VulkanResources::windowCloseStatus()
{
	return glfwWindowShouldClose(renderWindow);
}
