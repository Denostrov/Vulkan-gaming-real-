#include "VulkanResources.h"

using namespace std::literals;

//get required resources
template<class Source, class Resource>
auto enumerateRequiredFunc() {}
template<>
auto enumerateRequiredFunc<vk::Instance, vk::ExtensionProperties>()
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
auto enumerateRequiredFunc<vk::PhysicalDevice, vk::ExtensionProperties>()
{
	return std::vector<char const*>(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());
}
template<class Resource>
auto enumerateRequiredFunc() {}
template<>
auto enumerateRequiredFunc<vk::LayerProperties>()
{
	return std::vector<char const*>(VALIDATION_LAYERS.begin(), VALIDATION_LAYERS.end());
}

//get name of resource from struct
template<class Resource>
auto getResourceNameFunc(Resource const& val) {}
template<>
auto getResourceNameFunc(vk::ExtensionProperties const& val)
{
	return val.extensionName.data();
}
template<>
auto getResourceNameFunc(vk::LayerProperties const& val)
{
	return val.layerName.data();
}

//check each required resource for availability and print it
template<class Resource>
auto checkAndPrintRequired(std::vector<Resource> const& resources, std::vector<char const*> const& requiredResourceNames)
{
	for (auto name : requiredResourceNames)
	{
		bool isSupported = checkVectorContainsString(resources, getResourceNameFunc<Resource>, name);
		formatPrint(std::cout, "\t{}\t"s + (isSupported ? "(supported)"s : "(not supported)"s) + "\n"s, name);
		if (!isSupported) return false;
	}
	return true;
}

//get create info for debug utils messenger with all messages
auto getDebugUtilsMessengerCreateInfo()
{
	using enum vk::DebugUtilsMessageTypeFlagBitsEXT;
	using enum vk::DebugUtilsMessageSeverityFlagBitsEXT;
	if (ENABLE_VALIDATION_LAYERS)
	{
		std::unique_ptr<vk::DebugUtilsMessengerCreateInfoEXT> debugUtilsMessengerCreateInfo{new vk::DebugUtilsMessengerCreateInfoEXT({},
																																	 eError | eWarning | eInfo | eVerbose,
																																	 eGeneral | eValidation | ePerformance, debugCallback, nullptr)};
		return debugUtilsMessengerCreateInfo;
	}
	else
	{
		return std::unique_ptr<vk::DebugUtilsMessengerCreateInfoEXT>(nullptr);
	}
}

//get required validation layers that are supported
auto getValidationLayers()
{
	if (ENABLE_VALIDATION_LAYERS)
	{
		auto availableLayers = vk::enumerateInstanceLayerProperties();
		std::cout << getTotalString(availableLayers, "available"s) << availableLayers;

		auto requiredLayers = enumerateRequiredFunc<vk::LayerProperties>();
		std::cout << getTotalString(requiredLayers, "required"s, getFormatString<vk::LayerProperties>(requiredLayers));
		assert(checkAndPrintRequired(availableLayers, requiredLayers));

		return requiredLayers;
	}
	else
	{
		return std::vector<char const*>();
	}
}

auto getInstanceExtensions()
{
	auto availableInstanceExtensions = vk::enumerateInstanceExtensionProperties();
	std::cout << getTotalString(availableInstanceExtensions, "available"s, getFormatString<vk::Instance>(availableInstanceExtensions)) << availableInstanceExtensions;

	auto requiredInstanceExtensions = enumerateRequiredFunc<vk::Instance, vk::ExtensionProperties>();
	std::cout << getTotalString(requiredInstanceExtensions, "required"s, getFormatString<vk::Instance, vk::ExtensionProperties>(requiredInstanceExtensions));
	checkAndPrintRequired(availableInstanceExtensions, requiredInstanceExtensions);

	return requiredInstanceExtensions;
}

auto getQueueFamilyIndices(vk::PhysicalDevice const& physicalDevice, vk::SurfaceKHR const& surface)
{
	QueueFamilyIndices queueFamilyIndices{};
	auto queueFamilies = physicalDevice.getQueueFamilyProperties();
	std::cout << getTotalString(queueFamilies, "available"s, getFormatString(queueFamilies)) << queueFamilies;

	bool foundGraphicsQueueFamily = false;
	bool foundPresentationQueueFamily = false;
	for (uint64_t i = 0; i < queueFamilies.size() && (!foundGraphicsQueueFamily || !foundPresentationQueueFamily); i++)
	{
		if (!foundGraphicsQueueFamily && queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
		{
			queueFamilyIndices.graphicsFamily = uint32_t(i);
			foundGraphicsQueueFamily = true;
		}
		if (!foundPresentationQueueFamily && physicalDevice.getSurfaceSupportKHR(uint32_t(i), surface))
		{
			queueFamilyIndices.presentationFamily = uint32_t(i);
			foundPresentationQueueFamily = true;
		}
	}

	return std::pair<bool, QueueFamilyIndices>(foundGraphicsQueueFamily && foundPresentationQueueFamily, queueFamilyIndices);
}

auto getSwapChainSupportDetails(vk::PhysicalDevice const& physicalDevice, vk::SurfaceKHR const& surface)
{
	SwapChainSupportDetails swapChainSupportDetails{};
	swapChainSupportDetails.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
	std::cout << toString<vk::SurfaceCapabilitiesKHR>() + ":\n"s << getFormatString(swapChainSupportDetails.capabilities) << "\n"s;

	swapChainSupportDetails.formats = physicalDevice.getSurfaceFormatsKHR(surface);
	std::cout << getTotalString(swapChainSupportDetails.formats, "supported"s, getFormatString(swapChainSupportDetails.formats)) <<
		swapChainSupportDetails.formats;

	swapChainSupportDetails.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
	std::cout << getTotalString(swapChainSupportDetails.presentModes, "supported"s, getFormatString(swapChainSupportDetails.presentModes)) <<
		swapChainSupportDetails.presentModes;

	return swapChainSupportDetails;
}

auto rateDeviceScore(vk::PhysicalDevice const& physicalDevice, std::vector<char const*> const& requiredPhysicalDeviceExtensions, vk::SurfaceKHR const& surface)
{
	auto physicalDeviceProperties = physicalDevice.getProperties();

	uint64_t score = 1;
	uint64_t multiplier = 1;

	if (physicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
	{
		multiplier++;
	}
	score += physicalDeviceProperties.limits.maxImageDimension2D;
	score += physicalDeviceProperties.limits.maxBoundDescriptorSets;
	score += physicalDeviceProperties.limits.maxPushConstantsSize;
	score += physicalDeviceProperties.limits.maxDescriptorSetUniformBuffers;
	score += physicalDeviceProperties.limits.maxFramebufferHeight;
	score += physicalDeviceProperties.limits.maxSamplerAllocationCount;
	score += physicalDeviceProperties.limits.maxImageArrayLayers;

	if (score > std::numeric_limits<uint32_t>::max())
	{
		score = 1;
	}

	uint64_t requirementMultiplier = 1;
	auto availablePhysicalDeviceExtensions = physicalDevice.enumerateDeviceExtensionProperties();
	std::cout << getTotalString(availablePhysicalDeviceExtensions, "available"s, getFormatString<vk::PhysicalDevice>(availablePhysicalDeviceExtensions)) <<
		availablePhysicalDeviceExtensions;

	SwapChainSupportDetails swapChainSupportDetails{};
	std::cout << getTotalString(requiredPhysicalDeviceExtensions, "required"s, getFormatString<vk::PhysicalDevice, vk::ExtensionProperties>(requiredPhysicalDeviceExtensions));
	if (!checkAndPrintRequired(availablePhysicalDeviceExtensions, requiredPhysicalDeviceExtensions))
	{
		requirementMultiplier = 0;
	}
	else
	{
		swapChainSupportDetails = getSwapChainSupportDetails(physicalDevice, surface);
		if (swapChainSupportDetails.formats.empty() || swapChainSupportDetails.presentModes.empty()) requirementMultiplier = 0;
	}
	score *= requirementMultiplier;

	requirementMultiplier = 0;
	auto [result, queueFamilyIndices] = getQueueFamilyIndices(physicalDevice, surface);
	requirementMultiplier = result ? 1 : 0;
	score *= requirementMultiplier;

	return std::tuple<uint64_t, QueueFamilyIndices, SwapChainSupportDetails>(score * multiplier, queueFamilyIndices, swapChainSupportDetails);
}

auto choosePhysicalDevice(vk::Instance const& instance, std::vector<char const*> const& requiredPhysicalDeviceExtensions, vk::SurfaceKHR const& surface)
{
	auto physicalDevices = instance.enumeratePhysicalDevices();
	assert(!physicalDevices.empty() && "No devices with vulkan support found");

	uint64_t maxPhysicalDeviceScore = 0;
	vk::PhysicalDevice bestPhysicalDevice{};
	QueueFamilyIndices bestPhysicalDeviceQueueIndices{};
	std::cout << getTotalString(physicalDevices, "available"s);
	for (auto const& currentPhysicalDevice : physicalDevices)
	{
		auto physicalDeviceProperties = currentPhysicalDevice.getProperties();
		auto physicalDeviceFeatures = currentPhysicalDevice.getFeatures();
		std::cout << getFormatString(physicalDeviceProperties) << ",\t"s << getFormatString(physicalDeviceFeatures) << "\n"s;
		uint64_t currentPhysicalDeviceScore{};
		QueueFamilyIndices currentPhysicalDeviceQueueIndices{};
		SwapChainSupportDetails currentSwapChainSupportDetails{};
		std::tie(currentPhysicalDeviceScore, currentPhysicalDeviceQueueIndices, currentSwapChainSupportDetails) = rateDeviceScore(currentPhysicalDevice,
																														   requiredPhysicalDeviceExtensions, surface);
		std::cout << getLabelValuePairsString(LabelValuePair{"Device suitability score"s, currentPhysicalDeviceScore},
											  LabelValuePair{"Suitable queue family indices"s, currentPhysicalDeviceQueueIndices}) << "\n"s;
		if (currentPhysicalDeviceScore > maxPhysicalDeviceScore)
		{
			bestPhysicalDevice = currentPhysicalDevice;
			bestPhysicalDeviceQueueIndices = currentPhysicalDeviceQueueIndices;
			maxPhysicalDeviceScore = std::max(maxPhysicalDeviceScore, currentPhysicalDeviceScore);
		}
	}
	assert(maxPhysicalDeviceScore > 0 && "No suitable physical devices found");
	formatPrint(std::cout, "Picked {} as best physical device with {} score and {} queue family indices\n"sv,
				bestPhysicalDevice.getProperties().deviceName.data(), maxPhysicalDeviceScore, toString(bestPhysicalDeviceQueueIndices));
	return std::pair<vk::PhysicalDevice, QueueFamilyIndices>(bestPhysicalDevice, bestPhysicalDeviceQueueIndices);
}

VulkanResources::VulkanResources()
	:windowContext(),
	renderWindow(800, 600, windowContext)
{
	//load vulkan specific funcs into dispatcher
	vk::DynamicLoader dynamicLoader;
	auto vkGetInstanceProcAddr = dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

	auto validationLayers = getValidationLayers();
	auto instanceExtensions = getInstanceExtensions();
	auto debugUtilsMessengerCreateInfo = getDebugUtilsMessengerCreateInfo();

	auto requiredPhysicalDeviceExtensions = enumerateRequiredFunc<vk::PhysicalDevice, vk::ExtensionProperties>();

	instance = createInstance(validationLayers, instanceExtensions, debugUtilsMessengerCreateInfo.get());
	assert(instance && "couldn't create instance");
	formatPrint(std::cout, "Created an instance\n"sv);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.get());

	if (ENABLE_VALIDATION_LAYERS)
	{
		debugUtilsMessenger = createDebugUtilsMessenger(*debugUtilsMessengerCreateInfo);
		assert(debugUtilsMessenger && "couldn't create debug messenger");
		formatPrint(std::cout, "Created a debug messenger\n"sv);
	}

	surface = createSurface();
	assert(surface && "couldn't create surface");
	formatPrint(std::cout, "Created a window surface\n"sv);

	std::tie(physicalDevice, queueFamilyIndices) = choosePhysicalDevice(instance.get(), requiredPhysicalDeviceExtensions, surface.get());

	device = createDevice(validationLayers, requiredPhysicalDeviceExtensions);
	assert(device && "couldn't create logical device");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(device.get());
	formatPrint(std::cout, "Created logical device\n"sv);

	graphicsQueue = device.get().getQueue(queueFamilyIndices.graphicsFamily, 0);
	formatPrint(std::cout, "Acquired graphics queue\n"sv);
	presentationQueue = device.get().getQueue(queueFamilyIndices.presentationFamily, 0);
	formatPrint(std::cout, "Acquired presentation queue\n"sv);


}

VulkanResources::~VulkanResources()
{

}

bool VulkanResources::windowCloseStatus()
{
	return glfwWindowShouldClose(renderWindow);
}

vk::UniqueInstance VulkanResources::createInstance(std::vector<char const*> const& validationLayers, std::vector<char const*> const& instanceExtensions,
												   vk::DebugUtilsMessengerCreateInfoEXT* debugUtilsMessengerCreateInfo)
{
	uint32_t vulkanVersion = 0;
	if (VULKAN_HPP_DEFAULT_DISPATCHER.vkEnumerateInstanceVersion == nullptr)
	{
		vulkanVersion = VK_API_VERSION_1_0;
	}
	else
	{
		vulkanVersion = vk::enumerateInstanceVersion();
	}
	formatPrint(std::cout, "Vulkan instance version: {}.{}.{}.{}\n"sv, VK_API_VERSION_VARIANT(vulkanVersion), VK_API_VERSION_MAJOR(vulkanVersion),
				VK_API_VERSION_MINOR(vulkanVersion), VK_API_VERSION_PATCH(vulkanVersion));
	vk::ApplicationInfo applicationInfo{"Copesweeper", VK_MAKE_API_VERSION(0, 1, 0, 0), "Vulkan engine", VK_MAKE_API_VERSION(0, 1, 0, 0), vulkanVersion};

	vk::InstanceCreateInfo instanceCreateInfo{{}, &applicationInfo, validationLayers, instanceExtensions, debugUtilsMessengerCreateInfo};
	return vk::createInstanceUnique(instanceCreateInfo);
}

vk::UniqueDebugUtilsMessengerEXT VulkanResources::createDebugUtilsMessenger(vk::DebugUtilsMessengerCreateInfoEXT const& debugUtilsMessengerCreateInfo)
{
	return instance.get().createDebugUtilsMessengerEXTUnique(debugUtilsMessengerCreateInfo);
}

vk::UniqueSurfaceKHR VulkanResources::createSurface()
{
	VkSurfaceKHR windowSurface;
	glfwCreateWindowSurface(instance.get(), renderWindow, nullptr, &windowSurface);
	return vk::UniqueSurfaceKHR(vk::SurfaceKHR(windowSurface), instance.get());
}

vk::UniqueDevice VulkanResources::createDevice(std::vector<char const*> const& validationLayers, std::vector<char const*> const& requiredPhysicalDeviceExtensions)
{
	std::vector<float> queuePriorities = {1.0f};
	std::unordered_set<uint32_t> uniqueQueueFamilyIndices = {queueFamilyIndices.graphicsFamily, queueFamilyIndices.presentationFamily};
	std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos{};
	for (auto index : uniqueQueueFamilyIndices)
	{
		deviceQueueCreateInfos.push_back({{}, index, queuePriorities});
	}
	vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
	vk::DeviceCreateInfo deviceCreateInfo({}, deviceQueueCreateInfos, validationLayers, requiredPhysicalDeviceExtensions, &physicalDeviceFeatures);
	return physicalDevice.createDeviceUnique(deviceCreateInfo);
}

vk::UniqueSwapchainKHR VulkanResources::createSwapchain()
{
	return vk::UniqueSwapchainKHR();
}
