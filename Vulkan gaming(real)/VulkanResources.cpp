#include "VulkanResources.h"

#include <unordered_set>
#include <chrono>
#include <stb_image.h>

#include "helpers.h"
#include "logging.h"
#include "print.h"

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
		auto availableLayers = errorFatal(vk::enumerateInstanceLayerProperties(), "couldn't enumerate instance layer properties"s);
		std::cout << getTotalString(availableLayers, "available"s) << availableLayers;

		auto requiredLayers = enumerateRequiredFunc<vk::LayerProperties>();
		std::cout << getTotalString(requiredLayers, "required"s, getFormatString<vk::LayerProperties>(requiredLayers));
		errorFatal(checkAndPrintRequired(availableLayers, requiredLayers), "Not all validation layers supported"s);

		return requiredLayers;
	}
	else
	{
		return std::vector<char const*>();
	}
}

auto getInstanceExtensions()
{
	auto availableInstanceExtensions = errorFatal(vk::enumerateInstanceExtensionProperties(), "couldn't enumerate instance extension properties"s);
	std::cout << getTotalString(availableInstanceExtensions, "available"s, getFormatString<vk::Instance>(availableInstanceExtensions)) << availableInstanceExtensions;

	auto requiredInstanceExtensions = enumerateRequiredFunc<vk::Instance, vk::ExtensionProperties>();
	std::cout << getTotalString(requiredInstanceExtensions, "required"s, getFormatString<vk::Instance, vk::ExtensionProperties>(requiredInstanceExtensions));
	errorFatal(checkAndPrintRequired(availableInstanceExtensions, requiredInstanceExtensions), "Not all instance extensions available"s);

	return requiredInstanceExtensions;
}

auto VulkanResources::getQueueFamilyIndices(vk::PhysicalDevice physicalDevice)
{
	QueueFamilyIndices queueFamilyIndices{};
	auto queueFamilies = physicalDevice.getQueueFamilyProperties();
	std::cout << getTotalString(queueFamilies, "available"s, getFormatString(queueFamilies)) << queueFamilies;

	bool foundGraphicsQueueFamily = false;
	bool foundPresentationQueueFamily = false;
	for (size_t i = 0; i < queueFamilies.size() && (!foundGraphicsQueueFamily || !foundPresentationQueueFamily); i++)
	{
		if (!foundGraphicsQueueFamily && queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
		{
			queueFamilyIndices.graphicsFamily = uint32_t(i);
			foundGraphicsQueueFamily = true;
		}
		if (!foundPresentationQueueFamily && errorFatal(physicalDevice.getSurfaceSupportKHR(uint32_t(i), surface.get()), "couldn't get surface support"s))
		{
			queueFamilyIndices.presentationFamily = uint32_t(i);
			foundPresentationQueueFamily = true;
		}
	}

	return std::pair<bool, QueueFamilyIndices>(foundGraphicsQueueFamily && foundPresentationQueueFamily, queueFamilyIndices);
}

auto VulkanResources::getSwapchainSupportDetails(vk::PhysicalDevice const& physicalDevice)
{
	SwapchainSupportDetails swapchainSupportDetails{};
	swapchainSupportDetails.capabilities = errorFatal(physicalDevice.getSurfaceCapabilitiesKHR(surface.get()), "couldn't get surface capabilities"s);
	std::cout << toString<vk::SurfaceCapabilitiesKHR>() + ":\n"s << getFormatString(swapchainSupportDetails.capabilities) << "\n"s;

	swapchainSupportDetails.formats = errorFatal(physicalDevice.getSurfaceFormatsKHR(surface.get()), "couldn't get surface formats"s);
	std::cout << getTotalString(swapchainSupportDetails.formats, "supported"s, getFormatString(swapchainSupportDetails.formats)) <<
		swapchainSupportDetails.formats;

	swapchainSupportDetails.presentModes = errorFatal(physicalDevice.getSurfacePresentModesKHR(surface.get()), "couldn't get surface present modes"s);
	std::cout << getTotalString(swapchainSupportDetails.presentModes, "supported"s, getFormatString(swapchainSupportDetails.presentModes)) <<
		swapchainSupportDetails.presentModes;

	return swapchainSupportDetails;
}

auto VulkanResources::rateDeviceScore(vk::PhysicalDevice const& physicalDevice,
									  std::vector<char const*> const& requiredPhysicalDeviceExtensions)
{
	auto physicalDeviceProperties = physicalDevice.getProperties();
	auto physicalDeviceFeatures = physicalDevice.getFeatures();

	uint64_t score = 1;
	uint64_t multiplier = 1;

	if (physicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
	{
		multiplier++;
	}
	score += physicalDeviceProperties.limits.maxImageDimension1D;
	score += physicalDeviceProperties.limits.maxImageDimension2D;
	score += physicalDeviceProperties.limits.maxImageDimension3D;
	score += physicalDeviceProperties.limits.maxBoundDescriptorSets;
	score += physicalDeviceProperties.limits.maxPushConstantsSize;
	score += physicalDeviceProperties.limits.maxFramebufferHeight;
	score += physicalDeviceProperties.limits.maxImageArrayLayers;
	score += physicalDeviceProperties.limits.maxVertexInputBindings;
	score += physicalDeviceProperties.limits.maxMemoryAllocationCount;
	score += physicalDeviceProperties.limits.maxSamplerAllocationCount;

	if (score > std::numeric_limits<uint32_t>::max())
	{
		score = 1;
	}

	uint64_t requirementMultiplier = 1;
	auto availablePhysicalDeviceExtensions = errorFatal(physicalDevice.enumerateDeviceExtensionProperties(), "couldn't enumerate device extension properties"s);
	std::cout << getTotalString(availablePhysicalDeviceExtensions, "available"s, getFormatString<vk::PhysicalDevice>(availablePhysicalDeviceExtensions)) <<
		availablePhysicalDeviceExtensions;

	SwapchainSupportDetails swapchainSupportDetails{};
	std::cout << getTotalString(requiredPhysicalDeviceExtensions, "required"s, getFormatString<vk::PhysicalDevice, vk::ExtensionProperties>(requiredPhysicalDeviceExtensions));
	if (!checkAndPrintRequired(availablePhysicalDeviceExtensions, requiredPhysicalDeviceExtensions))
	{
		requirementMultiplier = 0;
	}
	else
	{
		swapchainSupportDetails = getSwapchainSupportDetails(physicalDevice);
		if (swapchainSupportDetails.formats.empty() || swapchainSupportDetails.presentModes.empty()) requirementMultiplier = 0;
	}
	score *= requirementMultiplier;

	requirementMultiplier = 0;
	auto [result, queueFamilyIndices] = getQueueFamilyIndices(physicalDevice);
	requirementMultiplier = result ? 1 : 0;
	score *= requirementMultiplier;

	score *= physicalDeviceFeatures.samplerAnisotropy ? 1 : 0;

	return std::make_tuple(score * multiplier, queueFamilyIndices, swapchainSupportDetails);
}

auto getOptionalPhysicalDeviceFeatures(vk::PhysicalDeviceFeatures const& supportedFeatures)
{
	vk::PhysicalDeviceFeatures result{};
	result.fillModeNonSolid = supportedFeatures.fillModeNonSolid;
	result.samplerAnisotropy = supportedFeatures.samplerAnisotropy;
	return result;
}

auto VulkanResources::choosePhysicalDevice(std::vector<char const*> const& requiredPhysicalDeviceExtensions)
{
	auto physicalDevices = errorFatal(instance->enumeratePhysicalDevices(), "couldn't enumerate physical devices"s);
	errorFatal(!physicalDevices.empty(), "No devices with vulkan support found"s);

	uint64_t maxPhysicalDeviceScore = 0;
	vk::PhysicalDevice bestPhysicalDevice{};
	QueueFamilyIndices bestPhysicalDeviceQueueIndices{};
	SwapchainSupportDetails bestSwapchainSupportDetails{};
	vk::PhysicalDeviceFeatures bestPhysicalDeviceFeatures{};
	std::cout << getTotalString(physicalDevices, "available"s);
	for (auto const& currentPhysicalDevice : physicalDevices)
	{
		auto physicalDeviceProperties = currentPhysicalDevice.getProperties();
		auto physicalDeviceFeatures = currentPhysicalDevice.getFeatures();
		auto physicalDeviceMemoryProperties = currentPhysicalDevice.getMemoryProperties();
		std::cout << getFormatString(physicalDeviceProperties) << ",\t"s << getFormatString(physicalDeviceFeatures) << ",\t"s <<
			getFormatString(physicalDeviceMemoryProperties) << "\n"s;
		uint64_t currentPhysicalDeviceScore{};
		QueueFamilyIndices currentPhysicalDeviceQueueIndices{};
		SwapchainSupportDetails currentSwapchainSupportDetails{};
		std::tie(currentPhysicalDeviceScore, currentPhysicalDeviceQueueIndices, currentSwapchainSupportDetails) = rateDeviceScore(currentPhysicalDevice,
																																  requiredPhysicalDeviceExtensions);
		std::cout << getLabelValuePairsString(LabelValuePair{"Device suitability score"s, currentPhysicalDeviceScore},
											  LabelValuePair{"Suitable queue family indices"s, currentPhysicalDeviceQueueIndices}) << "\n"s;
		if (currentPhysicalDeviceScore > maxPhysicalDeviceScore)
		{
			bestPhysicalDevice = currentPhysicalDevice;
			bestPhysicalDeviceQueueIndices = currentPhysicalDeviceQueueIndices;
			bestSwapchainSupportDetails = currentSwapchainSupportDetails;
			bestPhysicalDeviceFeatures = physicalDeviceFeatures;
			maxPhysicalDeviceScore = std::max(maxPhysicalDeviceScore, currentPhysicalDeviceScore);
		}
	}
	errorFatal(maxPhysicalDeviceScore > 0, "No suitable physical devices found"s);
	formatPrint(std::cout, "Picked {} as best physical device with {} score and {} queue family indices\n"sv,
				bestPhysicalDevice.getProperties().deviceName.data(), maxPhysicalDeviceScore, toString(bestPhysicalDeviceQueueIndices));
	return std::make_tuple(bestPhysicalDevice, bestPhysicalDeviceQueueIndices, bestSwapchainSupportDetails,
						   getOptionalPhysicalDeviceFeatures(bestPhysicalDeviceFeatures));
}

auto chooseSwapSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& surfaceFormats)
{
	vk::SurfaceFormatKHR chosenFormat{};
	for (auto const& format : surfaceFormats)
	{
		if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			chosenFormat = format;
			break;
		}
	}
	chosenFormat = surfaceFormats[0];
	formatPrint(std::cout, "Picked swap surface format: {}\n"sv, getFormatString(chosenFormat));
	return chosenFormat;
}

auto chooseSwapPresentMode(std::vector<vk::PresentModeKHR> const& presentModes)
{
	vk::PresentModeKHR chosenPresentMode{};
	chosenPresentMode = vk::PresentModeKHR::eFifo;
	for (auto const& mode : presentModes)
	{
		if (mode == vk::PresentModeKHR::eMailbox)
		{
			chosenPresentMode = mode;
			break;
		}
	}
	formatPrint(std::cout, "Picked swap present mode: {}\n"sv, getFormatString(chosenPresentMode));
	return chosenPresentMode;
}

auto chooseSwapExtent(GLFWwindow* window, vk::SurfaceCapabilitiesKHR const& capabilities)
{
	vk::Extent2D chosenExtent{};
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		chosenExtent = capabilities.currentExtent;
	}
	else
	{
		int width{}, height{};
		glfwGetFramebufferSize(window, &width, &height);
		chosenExtent = vk::Extent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
		chosenExtent.width = std::clamp(chosenExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		chosenExtent.height = std::clamp(chosenExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
	}
	formatPrint(std::cout, "Picked swap extent: {}\n"sv, toString(chosenExtent));
	return chosenExtent;
}

auto createInstance(std::vector<char const*> const& validationLayers, std::vector<char const*> const& instanceExtensions,
					vk::DebugUtilsMessengerCreateInfoEXT const& debugUtilsMessengerCreateInfo)
{
	uint32_t vulkanVersion = 0;
	if (VULKAN_HPP_DEFAULT_DISPATCHER.vkEnumerateInstanceVersion == nullptr)
	{
		vulkanVersion = VK_API_VERSION_1_0;
	}
	else
	{
		vulkanVersion = errorFatal(vk::enumerateInstanceVersion(), "couldn't enumerate instance version"s);
	}
	formatPrint(std::cout, "Vulkan instance version: {}.{}.{}.{}\n"sv, VK_API_VERSION_VARIANT(vulkanVersion), VK_API_VERSION_MAJOR(vulkanVersion),
				VK_API_VERSION_MINOR(vulkanVersion), VK_API_VERSION_PATCH(vulkanVersion));
	vk::ApplicationInfo applicationInfo{"Copesweeper", VK_MAKE_API_VERSION(0, 1, 0, 0), "Vulkan engine", VK_MAKE_API_VERSION(0, 1, 0, 0), vulkanVersion};

	vk::InstanceCreateInfo instanceCreateInfo{{}, &applicationInfo, validationLayers, instanceExtensions, &debugUtilsMessengerCreateInfo};
	return errorFatal(vk::createInstanceUnique(instanceCreateInfo), "couldn't create instance"s);
}

auto VulkanResources::createDebugUtilsMessenger(vk::DebugUtilsMessengerCreateInfoEXT const& debugUtilsMessengerCreateInfo)
{
	return errorFatal(instance->createDebugUtilsMessengerEXTUnique(debugUtilsMessengerCreateInfo), "couldn't create debug utils messenger"s);
}

auto VulkanResources::createSurface()
{
	VkSurfaceKHR windowSurface;
	auto result = glfwCreateWindowSurface(instance.get(), renderWindow, nullptr, &windowSurface);
	errorFatal(result, "couldn't create window surface"s);
	return vk::UniqueSurfaceKHR(vk::SurfaceKHR(windowSurface), instance.get());
}

auto VulkanResources::createDevice(std::vector<char const*> const& validationLayers, std::vector<char const*> const& requiredPhysicalDeviceExtensions)
{
	std::vector<float> queuePriorities{1.0f};
	std::unordered_set<uint32_t> uniqueQueueFamilyIndices = {queueFamilyIndices.graphicsFamily, queueFamilyIndices.presentationFamily};
	std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos{};
	for (auto index : uniqueQueueFamilyIndices)
	{
		deviceQueueCreateInfos.push_back({{}, index, queuePriorities});
	}
	vk::DeviceCreateInfo deviceCreateInfo({}, deviceQueueCreateInfos, validationLayers, requiredPhysicalDeviceExtensions, &supportedFeatures);
	return errorFatal(physicalDevice.createDeviceUnique(deviceCreateInfo), "couldn't create device"s);
}

auto VulkanResources::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
{
	vk::ImageSubresourceRange subresourceRange{aspectFlags, 0, 1, 0, 1};
	vk::ImageViewCreateInfo viewCreateInfo{{}, image, vk::ImageViewType::e2D, format,
										   {vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,vk::ComponentSwizzle::eIdentity,vk::ComponentSwizzle::eIdentity},
											subresourceRange};

	return errorFatal(device->createImageViewUnique(viewCreateInfo), "couldn't create image view"s);
}

auto VulkanResources::createSwapchain(SwapchainSupportDetails const& swapchainSupportDetails, vk::SwapchainKHR oldSwapchain)
{
	auto surfaceFormat = chooseSwapSurfaceFormat(swapchainSupportDetails.formats);
	auto presentMode = chooseSwapPresentMode(swapchainSupportDetails.presentModes);
	auto extent = chooseSwapExtent(renderWindow, swapchainSupportDetails.capabilities);

	uint32_t imageCount = swapchainSupportDetails.capabilities.minImageCount + 1;
	if (swapchainSupportDetails.capabilities.maxImageCount > 0 && imageCount > swapchainSupportDetails.capabilities.maxImageCount)
	{
		imageCount = swapchainSupportDetails.capabilities.maxImageCount;
	}

	vk::SharingMode imageSharingMode{};
	std::vector<uint32_t> queueIndices{};
	if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentationFamily)
	{
		imageSharingMode = vk::SharingMode::eConcurrent;
		queueIndices = {queueFamilyIndices.graphicsFamily, queueFamilyIndices.presentationFamily};
	}
	else
	{
		imageSharingMode = vk::SharingMode::eExclusive;
	}

	vk::SwapchainCreateInfoKHR createInfo{{}, surface.get(), imageCount, surfaceFormat.format, surfaceFormat.colorSpace, extent, 1,
		vk::ImageUsageFlagBits::eColorAttachment, imageSharingMode, queueIndices, swapchainSupportDetails.capabilities.currentTransform,
		vk::CompositeAlphaFlagBitsKHR::eOpaque, presentMode, vk::Bool32(true), oldSwapchain};

	auto newSwapchain = errorFatal(device->createSwapchainKHRUnique(createInfo), "couldn't create swapchain"s);
	auto newSwapChainImages = errorFatal(device->getSwapchainImagesKHR(newSwapchain.get()), "couldn't get swapchain images"s);

	return std::make_tuple(std::move(newSwapchain), newSwapChainImages, surfaceFormat.format, extent);
}

auto VulkanResources::createSwapchainImageViews(std::vector<vk::Image> const& swapchainImages, vk::Format swapchainImageFormat)
{
	std::vector<vk::UniqueImageView> result{};
	for (auto const& image : swapchainImages)
	{
		vk::ImageSubresourceRange subresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};
		vk::ImageViewCreateInfo imageViewCreateInfo({}, image, vk::ImageViewType::e2D, swapchainImageFormat,
													{vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,vk::ComponentSwizzle::eIdentity,vk::ComponentSwizzle::eIdentity},
													subresourceRange);
		result.push_back(createImageView(image, swapchainImageFormat, vk::ImageAspectFlagBits::eColor));
	}

	return result;
}

auto VulkanResources::createShaderModule(std::vector<char> const& shaderCode)
{
	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{{}, shaderCode.size(), reinterpret_cast<uint32_t const*>(shaderCode.data())};

	return errorFatal(device->createShaderModuleUnique(shaderModuleCreateInfo), "couldn't create shader module"s);
}

auto VulkanResources::findSupportedFormat(std::vector<vk::Format> const& candidateFormats, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
{
	for (auto candidate : candidateFormats)
	{
		auto properties = physicalDevice.getFormatProperties(candidate);

		if (tiling == vk::ImageTiling::eLinear && (properties.linearTilingFeatures & features) == features)
		{
			return candidate;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (properties.optimalTilingFeatures & features) == features)
		{
			return candidate;
		}
	}
	errorFatal(false, "couldn't find supported format"s);
	return vk::Format{};
}

auto VulkanResources::findDepthFormat()
{
	return findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal,
							   vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

auto VulkanResources::createRenderPass(vk::Format swapchainImageFormat)
{
	vk::AttachmentDescription colorAttachment{{}, swapchainImageFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
		vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR};

	vk::AttachmentReference colorAttachmentRef{0, vk::ImageLayout::eColorAttachmentOptimal};

	vk::AttachmentDescription depthAttachment{{}, findDepthFormat(), vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eDontCare,
		vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal};

	vk::AttachmentReference depthAttachmentRef{1, vk::ImageLayout::eDepthStencilAttachmentOptimal};

	vk::SubpassDescription subpass{{}, vk::PipelineBindPoint::eGraphics, nullptr, colorAttachmentRef, {}, &depthAttachmentRef};

	vk::SubpassDependency subpassDependency{VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		vk::AccessFlagBits{0}, vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite};

	std::vector attachments{colorAttachment, depthAttachment};
	vk::RenderPassCreateInfo renderPassCreateInfo{{}, attachments, subpass, subpassDependency};
	return errorFatal(device->createRenderPassUnique(renderPassCreateInfo), "couldn't create render pass"s);
}

auto VulkanResources::createDescriptorSetLayout()
{
	vk::DescriptorSetLayoutBinding uboLayoutBinding{0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex};

	vk::DescriptorSetLayoutBinding samplerLayoutBinding{1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment};

	std::vector bindings{uboLayoutBinding, samplerLayoutBinding};

	vk::DescriptorSetLayoutCreateInfo layoutCreateInfo{{}, bindings};
	return errorFatal(device->createDescriptorSetLayoutUnique(layoutCreateInfo));
}

auto VulkanResources::createGraphicsPipelineLayout()
{
	vk::PipelineLayoutCreateInfo layoutCreateInfo{{}, descriptorSetLayout.get(), nullptr};
	return errorFatal(device->createPipelineLayoutUnique(layoutCreateInfo), "couldn't create pipeline layout"s);
}

auto VulkanResources::createGraphicsPipeline(vk::Extent2D viewportExtent, vk::RenderPass renderPass, vk::PolygonMode polygonMode)
{
	auto vertexShaderCode = readFile("shaders/vertex.spv");
	auto fragmentShaderCode = readFile("shaders/fragment.spv");
	errorFatal(!vertexShaderCode.empty() && !fragmentShaderCode.empty(), "couldn't read shader files"s);

	vk::UniqueShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
	formatPrint(std::cout, "Created vertex shader module\n"sv);
	vk::UniqueShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);
	formatPrint(std::cout, "Created fragment shader module\n"sv);

	vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo{{}, vk::ShaderStageFlagBits::eVertex, vertexShaderModule.get(), "main"};
	vk::PipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{{}, vk::ShaderStageFlagBits::eFragment, fragmentShaderModule.get(), "main"};

	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages{vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo};

	std::vector<vk::DynamicState> dynamicStates{vk::DynamicState::eViewport, vk::DynamicState::eScissor};

	vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo{{}, dynamicStates};

	std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions = {Vertex::getBindingDescription(), InstanceVertex::getBindingDescription()};
	auto totalInputAttributeDescriptions = Vertex::getAttributeDescriptions();
	auto instanceInputAttributeDescriptions = InstanceVertex::getAttributeDescriptions();
	totalInputAttributeDescriptions.insert(totalInputAttributeDescriptions.end(), instanceInputAttributeDescriptions.begin(), instanceInputAttributeDescriptions.end());
	vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{{}, vertexInputBindingDescriptions, totalInputAttributeDescriptions};

	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{{}, vk::PrimitiveTopology::eTriangleStrip, VK_FALSE};

	vk::Viewport viewport{0.0f, 0.0f, static_cast<float>(viewportExtent.width),
		static_cast<float>(viewportExtent.height), 0.0f, 1.0f};
	vk::Rect2D scissor{{0, 0}, viewportExtent};

	vk::PipelineViewportStateCreateInfo viewportStateCreateInfo{{}, viewport, scissor};

	vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{{}, VK_FALSE, VK_FALSE, polygonMode, vk::CullModeFlagBits::eBack,
		vk::FrontFace::eClockwise, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f};

	vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo{{}, vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr, VK_FALSE, VK_FALSE};

	vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{VK_FALSE, vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
		vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
		vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};

	vk::PipelineDepthStencilStateCreateInfo depthStencilCreateInfo{{}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f};

	vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{{}, VK_FALSE, vk::LogicOp::eCopy, colorBlendAttachmentState, {0.0f, 0.0f, 0.0f, 0.0f}};

	vk::GraphicsPipelineCreateInfo pipelineCreateInfo{{}, shaderStages, &vertexInputStateCreateInfo, &inputAssemblyStateCreateInfo, nullptr,
		&viewportStateCreateInfo, &rasterizationStateCreateInfo, &multisampleStateCreateInfo, &depthStencilCreateInfo, &colorBlendStateCreateInfo,
		&dynamicStateCreateInfo, pipelineLayout.get(), renderPass, 0};
	auto pipeline = errorFatal(device->createGraphicsPipelineUnique(nullptr, pipelineCreateInfo), "couldn't create graphics pipeline"s);
	return pipeline;
}

auto VulkanResources::createFramebuffers(SwapchainResources const& swapchainResources)
{
	std::vector<vk::UniqueFramebuffer> framebuffers;

	for (size_t i = 0; i < swapchainResources.swapchainImageViews.size(); i++)
	{
		std::vector attachments{swapchainResources.swapchainImageViews[i].get(), swapchainResources.depthImageViews[i].get()};

		vk::FramebufferCreateInfo framebufferCreateInfo{{}, swapchainResources.renderPass.get(), attachments, swapchainResources.swapchainExtent.width,
			swapchainResources.swapchainExtent.height, 1};
		framebuffers.push_back(errorFatal(device->createFramebufferUnique(framebufferCreateInfo), "couldn't create framebuffer"s));
	}
	return framebuffers;
}

auto VulkanResources::createCommandPool(vk::CommandPoolCreateFlags flags)
{
	vk::CommandPoolCreateInfo commandPoolCreateInfo{flags, queueFamilyIndices.graphicsFamily};
	return errorFatal(device->createCommandPoolUnique(commandPoolCreateInfo), "couldn't create command pool"s);
}

auto VulkanResources::beginSingleTimeCommands()
{
	vk::CommandBufferAllocateInfo allocateInfo{shortBufferCommandPool.get(), vk::CommandBufferLevel::ePrimary, 1};

	auto commandBuffer = errorFatal(device->allocateCommandBuffersUnique(allocateInfo), "couldn't allocate copy command buffer"s);

	vk::CommandBufferBeginInfo beginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit};

	errorFatal(commandBuffer[0]->begin(beginInfo), "couldn't begin command buffer"s);

	return std::move(commandBuffer[0]);
}

auto VulkanResources::endSingleTimeCommands(vk::CommandBuffer commandBuffer)
{
	errorFatal(commandBuffer.end(), "couldn't end command buffer"s);

	vk::SubmitInfo submitInfo{{}, {}, commandBuffer};

	errorFatal(graphicsQueue.submit(submitInfo), "couldn't submit to graphics queue"s);
	errorFatal(graphicsQueue.waitIdle(), "couldn't wait for graphics queue to become idle"s);
}

auto findMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags memoryPropertyFlags)
{
	auto memoryProperties = physicalDevice.getMemoryProperties();
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
		{
			return i;
		}
	}

	errorFatal(false, "couldn't find memory type"s);
	return 0U;
}

auto VulkanResources::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags bufferUsage, vk::MemoryPropertyFlags memoryProperties)
{
	vk::BufferCreateInfo bufferInfo{{}, size, bufferUsage, vk::SharingMode::eExclusive};
	auto buffer = errorFatal(device->createBufferUnique(bufferInfo), "couldn't create vertex buffer"s);

	auto memoryRequirements = device->getBufferMemoryRequirements(buffer.get());

	vk::MemoryAllocateInfo memoryAllocateInfo{memoryRequirements.size, findMemoryType(physicalDevice, memoryRequirements.memoryTypeBits,
																					  memoryProperties)};
	auto bufferMemory = errorFatal(device->allocateMemoryUnique(memoryAllocateInfo), "couldn't allocate vertex buffer memory"s);

	errorFatal(device->bindBufferMemory(buffer.get(), bufferMemory.get(), 0), "couldn't bind buffer memory"s);

	return std::make_tuple(std::move(buffer), std::move(bufferMemory));
}

auto VulkanResources::copyBuffer(vk::Buffer sourceBuffer, vk::Buffer destBuffer, vk::DeviceSize size)
{
	auto commandBuffer = beginSingleTimeCommands();

	vk::BufferCopy bufferCopy{0, 0, size};
	commandBuffer->copyBuffer(sourceBuffer, destBuffer, bufferCopy);

	endSingleTimeCommands(commandBuffer.get());
}

auto VulkanResources::createUniformBuffers()
{
	std::vector<vk::UniqueBuffer> buffers(MAX_FRAMES_IN_FLIGHT);
	std::vector<vk::UniqueDeviceMemory> buffersMemory(MAX_FRAMES_IN_FLIGHT);
	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		std::tie(buffers[i], buffersMemory[i]) = createBuffer(sizeof(UniformBufferObject), vk::BufferUsageFlagBits::eUniformBuffer,
															  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
	}
	return std::make_tuple(std::move(buffers), std::move(buffersMemory));
}

auto VulkanResources::createDescriptorPool()
{
	vk::DescriptorPoolSize uniformPoolSize{vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT};
	vk::DescriptorPoolSize samplerPoolSize{vk::DescriptorType::eCombinedImageSampler, MAX_FRAMES_IN_FLIGHT};

	std::vector poolSizes{uniformPoolSize, samplerPoolSize};

	vk::DescriptorPoolCreateInfo poolCreateInfo{{}, MAX_FRAMES_IN_FLIGHT, poolSizes};
	return errorFatal(device->createDescriptorPoolUnique(poolCreateInfo), "couldn't create descriptor pool"s);
}

auto VulkanResources::createDescriptorSets()
{
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout.get());

	vk::DescriptorSetAllocateInfo allocateInfo{descriptorPool.get(), descriptorSetLayouts};
	auto descriptorSets = errorFatal(device->allocateDescriptorSets(allocateInfo), "couldn't allocate descriptor sets"s);

	for (uint64_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vk::DescriptorBufferInfo bufferInfo{uniformBuffers[i].get(), 0, VK_WHOLE_SIZE};

		vk::DescriptorImageInfo imageInfo{textureSampler.get(), textureImageView.get(), vk::ImageLayout::eShaderReadOnlyOptimal};

		vk::WriteDescriptorSet bufferDescriptorWrite{descriptorSets[i], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo};

		vk::WriteDescriptorSet imageDescriptorWrite{descriptorSets[i], 1, 0, vk::DescriptorType::eCombinedImageSampler, imageInfo};

		std::vector descriptorWrites{bufferDescriptorWrite, imageDescriptorWrite};

		device->updateDescriptorSets(descriptorWrites, {});
	}

	return descriptorSets;
}

template<class Data>
auto VulkanResources::createStagingBuffer(Data* data, vk::DeviceSize size)
{
	auto [stagingBuffer, stagingMemory] = createBuffer(size, vk::BufferUsageFlagBits::eTransferSrc,
													   vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

	auto dataTarget = errorFatal(device->mapMemory(stagingMemory.get(), 0, size, {}), "couldn't map buffer memory"s);
	memcpy(dataTarget, data, size);
	device->unmapMemory(stagingMemory.get());

	return std::make_tuple(std::move(stagingBuffer), std::move(stagingMemory));
}

template<class Data>
auto VulkanResources::createDeviceLocalBuffer(Data const& arr, vk::BufferUsageFlags bufferUsage)
{
	vk::DeviceSize bufferSize = sizeof(arr[0]) * arr.size();

	auto [stagingBuffer, stagingMemory] = createStagingBuffer(arr.data(), bufferSize);

	auto [finalBuffer, finalMemory] = createBuffer(bufferSize, bufferUsage | vk::BufferUsageFlagBits::eTransferDst,
												   vk::MemoryPropertyFlagBits::eDeviceLocal);

	copyBuffer(stagingBuffer.get(), finalBuffer.get(), bufferSize);

	return std::make_tuple(std::move(finalBuffer), std::move(finalMemory));
}

auto VulkanResources::createHostVisibleBuffer(vk::DeviceSize size, vk::BufferUsageFlags bufferUsage)
{
	return createBuffer(size, bufferUsage, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
}

auto VulkanResources::createInstanceVertexBuffers()
{
	std::vector<vk::UniqueBuffer> buffers(MAX_FRAMES_IN_FLIGHT);
	std::vector<vk::UniqueDeviceMemory> buffersMemory(MAX_FRAMES_IN_FLIGHT);
	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		std::tie(buffers[i], buffersMemory[i]) = createHostVisibleBuffer(sizeof(InstanceVertex) * 1024, vk::BufferUsageFlagBits::eVertexBuffer);
	}
	return std::make_tuple(std::move(buffers), std::move(buffersMemory));
}

auto VulkanResources::copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height)
{
	auto commandBuffer = beginSingleTimeCommands();

	vk::ImageSubresourceLayers subresourceLayers{vk::ImageAspectFlagBits::eColor, 0, 0, 1};
	vk::BufferImageCopy bufferImageCopy{0, 0, 0, subresourceLayers, {0, 0, 0}, {width, height, 1}};

	commandBuffer->copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, bufferImageCopy);

	endSingleTimeCommands(commandBuffer.get());
}

auto hasStencilComponent(vk::Format format)
{
	return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}

auto VulkanResources::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{
	auto commandBuffer = beginSingleTimeCommands();

	vk::AccessFlags srcAccessMask;
	vk::AccessFlags dstAccessMask;
	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destinationStage;
	vk::ImageAspectFlags aspectMask;
	if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
	{
		aspectMask = vk::ImageAspectFlagBits::eDepth;
		if (hasStencilComponent(format))
		{
			aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
	}
	else
	{
		aspectMask = vk::ImageAspectFlagBits::eColor;
	}

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
	{
		srcAccessMask = {};
		dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
	{
		srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
	}
	else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
	{
		srcAccessMask = {};
		dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
	}
	else
	{
		errorFatal(false, "unknown image layout transition"s);
	}

	vk::ImageSubresourceRange imageSubresourceRange{aspectMask, 0, 1, 0, 1};
	vk::ImageMemoryBarrier imageMemoryBarrier{srcAccessMask, dstAccessMask, oldLayout, newLayout, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
		image, imageSubresourceRange};

	commandBuffer->pipelineBarrier(sourceStage, destinationStage, {}, {}, {}, imageMemoryBarrier);

	endSingleTimeCommands(commandBuffer.get());
}

auto VulkanResources::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage,
								  vk::MemoryPropertyFlags properties)
{
	vk::ImageCreateInfo imageCreateInfo{{},vk::ImageType::e2D, format, {width, height, 1}, 1, 1,
		vk::SampleCountFlagBits::e1, tiling, usage, vk::SharingMode::eExclusive,
		{}, vk::ImageLayout::eUndefined};

	auto textureImage = errorFatal(device->createImageUnique(imageCreateInfo), "couldn't create image"s);

	auto memoryRequirements = device->getImageMemoryRequirements(textureImage.get());

	vk::MemoryAllocateInfo memoryAllocateInfo{memoryRequirements.size,
		findMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, properties)};

	auto textureImageMemory = errorFatal(device->allocateMemoryUnique(memoryAllocateInfo), "couldn't allocate texture image memory"s);
	errorFatal(device->bindImageMemory(textureImage.get(), textureImageMemory.get(), 0), "couldn't bind image memory"s);

	return std::make_tuple(std::move(textureImage), std::move(textureImageMemory));
}

auto VulkanResources::createTextureImage()
{
	int textureWidth{}, textureHeight{}, textureChannels{};
	stbi_uc* pixels = stbi_load("textures/DejaVu mono.png", &textureWidth, &textureHeight,
								&textureChannels, STBI_rgb_alpha);
	vk::DeviceSize imageSize = textureWidth * textureHeight * 4;

	errorFatal(pixels, "couldn't load texture image"s);

	auto [stagingBuffer, stagingBufferMemory] = createStagingBuffer(pixels, imageSize);

	stbi_image_free(pixels);

	auto [textureImage, textureImageMemory] = createImage((uint32_t)textureWidth, (uint32_t)textureHeight, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal,
														  vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);

	transitionImageLayout(textureImage.get(), vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
	copyBufferToImage(stagingBuffer.get(), textureImage.get(), (uint32_t)textureWidth, (uint32_t)textureHeight);
	transitionImageLayout(textureImage.get(), vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

	return std::make_tuple(std::move(textureImage), std::move(textureImageMemory));
}

auto VulkanResources::createTextureImageView()
{
	return createImageView(textureImage.get(), vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);
}

auto VulkanResources::createTextureSampler()
{
	auto maxAnisotropy = physicalDevice.getProperties().limits.maxSamplerAnisotropy;

	vk::SamplerCreateInfo samplerCreateInfo{{}, vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear, vk::SamplerAddressMode::eClampToEdge,
		vk::SamplerAddressMode::eClampToEdge, vk::SamplerAddressMode::eClampToEdge, 0.0f, VK_TRUE, maxAnisotropy, VK_FALSE, vk::CompareOp::eAlways,
		0.0f, 0.0f, vk::BorderColor::eIntOpaqueBlack, VK_FALSE};

	return errorFatal(device->createSamplerUnique(samplerCreateInfo), "couldn't create texture sampler"s);
}

auto VulkanResources::createDepthResources(SwapchainResources const& swapchainResources)
{
	auto depthFormat = findDepthFormat();
	std::vector<vk::UniqueImage> depthImages;
	std::vector<vk::UniqueDeviceMemory> depthImagesMemory;
	std::vector<vk::UniqueImageView> depthImageViews;

	for (uint64_t i = 0; i < swapchainResources.swapchainImages.size(); i++)
	{
		auto [depthImage, depthMemory] = createImage(swapchainResources.swapchainExtent.width, swapchainResources.swapchainExtent.height,
													 depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal);

		auto depthImageView = createImageView(depthImage.get(), depthFormat, vk::ImageAspectFlagBits::eDepth);

		transitionImageLayout(depthImage.get(), depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);
		depthImages.push_back(std::move(depthImage));
		depthImagesMemory.push_back(std::move(depthMemory));
		depthImageViews.push_back(std::move(depthImageView));
	}

	return std::make_tuple(std::move(depthImages), std::move(depthImagesMemory), std::move(depthImageViews));
}

auto VulkanResources::createCommandBuffers()
{
	vk::CommandBufferAllocateInfo commandBufferAllocateInfo{commandPool.get(), vk::CommandBufferLevel::ePrimary, MAX_FRAMES_IN_FLIGHT};
	return errorFatal(device->allocateCommandBuffers(commandBufferAllocateInfo), "couldn't allocate command buffers"s);
}

auto VulkanResources::updateUniformBuffer(uint64_t frameIndex)
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();

	auto elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	UniformBufferObject vp{glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 100.0f)};
	vp.vp *= glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	auto data = errorFatal(device->mapMemory(uniformBuffersMemory[frameIndex].get(), 0, sizeof(vp)), "couldn't map memory"s);
	memcpy(data, &vp, sizeof(vp));
	device->unmapMemory(uniformBuffersMemory[frameIndex].get());
}

auto VulkanResources::updateInstanceBuffer(uint64_t frameIndex)
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();

	auto elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	if (ObjectPools::quads.size() > 0)
	{
		auto data = static_cast<InstanceVertex*>(errorFatal(device->mapMemory(instanceVertexBufferMemory[frameIndex].get(),
																			  0, sizeof(InstanceVertex) * ObjectPools::quads.size()), "couldn't map memory"s));

		memcpy(data, ObjectPools::quads.data(), sizeof(InstanceVertex) * ObjectPools::quads.size());

		device->unmapMemory(instanceVertexBufferMemory[frameIndex].get());
	}
}

auto VulkanResources::recordCommandBuffer(uint32_t imageIndex, SwapchainResources const& swapchainResources)
{
	vk::CommandBufferBeginInfo commandBufferBeginInfo{{}, nullptr};
	auto& commandBuffer = commandBuffers[currentFrame];

	errorFatal(commandBuffer.begin(commandBufferBeginInfo) == vk::Result::eSuccess, "couldn't begin command buffer"s);

	std::vector<vk::ClearValue> clearValues{vk::ClearColorValue{std::array{0.0f, 0.0f, 0.0f, 1.0f}}, vk::ClearDepthStencilValue{1.0f, 0}};
	vk::RenderPassBeginInfo renderPassBeginInfo{swapchainResources.renderPass.get(), swapchainResources.swapchainFramebuffers[imageIndex].get(),
												{{0, 0}, swapchainResources.swapchainExtent}, clearValues};

	commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, swapchainResources.graphicsPipelines.current());

	commandBuffer.bindVertexBuffers(0, vertexBuffer.get(), 0ULL);

	commandBuffer.bindVertexBuffers(1, instanceVertexBuffer[currentFrame].get(), 0ULL);

	commandBuffer.bindIndexBuffer(indexBuffer.get(), 0, vk::IndexType::eUint16);

	vk::Viewport viewport{0.0f, 0.0f, static_cast<float>(swapchainResources.swapchainExtent.width),
		static_cast<float>(swapchainResources.swapchainExtent.height), 0.0f, 1.0f};
	commandBuffer.setViewport(0, viewport);

	vk::Rect2D scissor{{0, 0}, swapchainResources.swapchainExtent};
	commandBuffer.setScissor(0, scissor);

	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSets[currentFrame], {});

	commandBuffer.drawIndexed(static_cast<uint32_t>(indices.size()), static_cast<uint32_t>(ObjectPools::quads.size()), 0, 0, 0);

	commandBuffer.endRenderPass();

	errorFatal(commandBuffer.end() == vk::Result::eSuccess, "couldn't end command buffer"s);
}

auto VulkanResources::createSyncObjects()
{
	vk::SemaphoreCreateInfo semaphoreCreateInfo{{}, nullptr};
	vk::FenceCreateInfo fenceCreateInfo{vk::FenceCreateFlagBits::eSignaled, nullptr};

	std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
	std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
	std::vector<vk::UniqueFence> inFlightFences;
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		imageAvailableSemaphores.push_back(errorFatal(device->createSemaphoreUnique(semaphoreCreateInfo), "couldn't create semaphore"s));
		renderFinishedSemaphores.push_back(errorFatal(device->createSemaphoreUnique(semaphoreCreateInfo), "couldn't create semaphore"s));
		inFlightFences.push_back(errorFatal(device->createFenceUnique(fenceCreateInfo), "couldn't create fence"s));
	}
	return std::make_tuple(std::move(imageAvailableSemaphores), std::move(renderFinishedSemaphores), std::move(inFlightFences));
}

SwapchainResources::SwapchainResources(VulkanResources& vulkan, RenderingPipelines::Type initialType, vk::SwapchainKHR oldSwapchain)
{
	std::tie(swapchain, swapchainImages, swapchainImageFormat, swapchainExtent) = vulkan.createSwapchain(vulkan.getSwapchainSupportDetails(vulkan.physicalDevice),
																										 oldSwapchain);
	formatPrint(std::cout, "Created swapchain\n"sv);
	formatPrint(std::cout, "{} swapchain images acquired\n"sv, swapchainImages.size());

	swapchainImageViews = vulkan.createSwapchainImageViews(swapchainImages, swapchainImageFormat);
	formatPrint(std::cout, "Created {} swapchain image views\n"sv, swapchainImageViews.size());

	renderPass = vulkan.createRenderPass(swapchainImageFormat);
	formatPrint(std::cout, "Created renderpass\n"sv);

	std::tie(depthImages, depthImagesMemory, depthImageViews) = vulkan.createDepthResources(*this);

	swapchainFramebuffers = vulkan.createFramebuffers(*this);
	formatPrint(std::cout, "Created {} framebuffers\n"sv, swapchainFramebuffers.size());

	graphicsPipelines = RenderingPipelines(vulkan, swapchainExtent, renderPass.get(), initialType);
	formatPrint(std::cout, "Created {} graphics pipelines\n"sv, graphicsPipelines.size());
}

template<class T>
void OldResourceQueue<T>::addToCleanup(std::unique_ptr<T>&& res, uint64_t waitCount)
{
	oldResources.push_back({std::move(res), waitCount});
}

template<class T>
void OldResourceQueue<T>::updateCleanup()
{
	if (!oldResources.empty())
	{
		std::for_each(oldResources.begin(), oldResources.end(), [](OldResourceQueue<T>::OldResource& p) { --p.conditionsLeft; });
		std::erase_if(oldResources, [](OldResourceQueue<T>::OldResource const& p) { return p.conditionsLeft == 0; });
	}
}

RenderingPipelines::RenderingPipelines(VulkanResources& vulkan, vk::Extent2D viewportExtent, vk::RenderPass renderPass, RenderingPipelines::Type initialType)
{
	pipelines.push_back(vulkan.createGraphicsPipeline(viewportExtent, renderPass, vk::PolygonMode::eFill));
	if (vulkan.supportedFeatures.fillModeNonSolid)
	{
		pipelines.push_back(vulkan.createGraphicsPipeline(viewportExtent, renderPass, vk::PolygonMode::eLine));
	}
	switchPipeline(initialType);
}

VulkanResources::VulkanResources(EventHandler* eventHandler)
	:windowContext(),
	renderWindow(800, 800, windowContext, eventHandler)
{
	//load vulkan specific funcs into dispatcher
	vk::DynamicLoader dynamicLoader;
	auto vkGetInstanceProcAddr = dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

	auto validationLayers = getValidationLayers();
	auto instanceExtensions = getInstanceExtensions();
	auto debugUtilsMessengerCreateInfo = getDebugUtilsMessengerCreateInfo();

	auto requiredPhysicalDeviceExtensions = enumerateRequiredFunc<vk::PhysicalDevice, vk::ExtensionProperties>();

	instance = createInstance(validationLayers, instanceExtensions, *debugUtilsMessengerCreateInfo);
	formatPrint(std::cout, "Created an instance\n"sv);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.get());

	if (ENABLE_VALIDATION_LAYERS)
	{
		debugUtilsMessenger = createDebugUtilsMessenger(*debugUtilsMessengerCreateInfo);
		formatPrint(std::cout, "Created a debug messenger\n"sv);
	}

	surface = createSurface();
	formatPrint(std::cout, "Created a window surface\n"sv);

	SwapchainSupportDetails swapchainSupportDetails{};
	std::tie(physicalDevice, queueFamilyIndices, swapchainSupportDetails, supportedFeatures) = choosePhysicalDevice(requiredPhysicalDeviceExtensions);

	device = createDevice(validationLayers, requiredPhysicalDeviceExtensions);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(device.get());
	formatPrint(std::cout, "Created logical device\n"sv);

	graphicsQueue = device->getQueue(queueFamilyIndices.graphicsFamily, 0);
	formatPrint(std::cout, "Acquired graphics queue\n"sv);
	presentationQueue = device->getQueue(queueFamilyIndices.presentationFamily, 0);
	formatPrint(std::cout, "Acquired presentation queue\n"sv);

	descriptorSetLayout = createDescriptorSetLayout();
	formatPrint(std::cout, "Created descriptor set layout"sv);

	pipelineLayout = createGraphicsPipelineLayout();
	formatPrint(std::cout, "Created graphics pipeline layout\n"sv);

	shortBufferCommandPool = createCommandPool(vk::CommandPoolCreateFlagBits::eTransient);
	formatPrint(std::cout, "Created short buffer command pool\n"sv);

	swapchainResources = std::make_unique<SwapchainResources>(*this, RenderingPipelines::Type::Main);

	commandPool = createCommandPool(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
	formatPrint(std::cout, "Created command pool\n"sv);

	std::tie(textureImage, textureImageMemory) = createTextureImage();
	formatPrint(std::cout, "Created texture image\n"sv);

	textureImageView = createTextureImageView();
	formatPrint(std::cout, "Created texture image view\n"sv);

	textureSampler = createTextureSampler();
	formatPrint(std::cout, "Created texture sampler\n"sv);

	std::tie(vertexBuffer, vertexBufferMemory) = createDeviceLocalBuffer(vertices, vk::BufferUsageFlagBits::eVertexBuffer);
	formatPrint(std::cout, "Created vertex buffer\n"sv);

	std::tie(instanceVertexBuffer, instanceVertexBufferMemory) = createInstanceVertexBuffers();
	formatPrint(std::cout, "Created instance vertex buffer\n"sv);

	std::tie(indexBuffer, indexBufferMemory) = createDeviceLocalBuffer(indices, vk::BufferUsageFlagBits::eIndexBuffer);
	formatPrint(std::cout, "Created index buffer\n"sv);

	std::tie(uniformBuffers, uniformBuffersMemory) = createUniformBuffers();
	formatPrint(std::cout, "Created {} uniform buffers\n"sv, uniformBuffers.size());

	descriptorPool = createDescriptorPool();
	formatPrint(std::cout, "Created descriptor pool\n"sv);

	descriptorSets = createDescriptorSets();
	formatPrint(std::cout, "Created descriptor sets\n"sv);

	commandBuffers = createCommandBuffers();
	formatPrint(std::cout, "Allocated command buffer\n"sv);

	std::tie(imageAvailableSemaphores, renderFinishedSemaphores, inFlightFences) = createSyncObjects();
	formatPrint(std::cout, "Created synchronization resources\n"sv);
}

bool VulkanResources::windowCloseStatus()
{
	return glfwWindowShouldClose(renderWindow);
}

void VulkanResources::setWindowShouldClose()
{
	glfwSetWindowShouldClose(renderWindow, true);
}

void VulkanResources::drawFrame()
{
	auto waitResult = device->waitForFences(inFlightFences[currentFrame].get(), VK_TRUE, std::numeric_limits<uint64_t>::max());

	oldSwapchainResources.updateCleanup();

	auto [acquireResult, imageIndex] = device->acquireNextImageKHR(swapchainResources->swapchain.get(), std::numeric_limits<uint64_t>::max(),
																   imageAvailableSemaphores[currentFrame].get());
	if (acquireResult == vk::Result::eErrorOutOfDateKHR || framebufferResized)
	{
		framebufferResized = false;
		recreateSwapchainResources();
		return;
	}
	else if (acquireResult == vk::Result::eSuboptimalKHR)
	{
		framebufferResized = false;
		recreateSwapchainResources();
		submitImage(oldSwapchainResources[oldSwapchainResources.size() - 1], imageIndex, true);
	}
	else if (acquireResult != vk::Result::eSuccess)
	{
		errorFatal(acquireResult, "couldn't acquire next image"s);
	}
	else
	{
		submitImage(*swapchainResources, imageIndex);
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanResources::stopRendering()
{
	errorFatal(device->waitIdle(), "couldn't wait for device idle"s);
}

void VulkanResources::recreateSwapchainResources()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(renderWindow, &width, &height);
	while (width == 0 || height == 0)
	{
		glfwWaitEvents();
		glfwGetFramebufferSize(renderWindow, &width, &height);
	}

	auto newSwapchainResources = std::make_unique<SwapchainResources>(*this, swapchainResources->graphicsPipelines.type(),
																	  swapchainResources->swapchain.get());
	oldSwapchainResources.addToCleanup(std::move(swapchainResources), MAX_FRAMES_IN_FLIGHT + 1);
	swapchainResources = std::move(newSwapchainResources);
}

void VulkanResources::submitImage(SwapchainResources const& swapchainResources, uint32_t imageIndex, bool isSwapchainRetired)
{
	updateUniformBuffer(currentFrame);
	updateInstanceBuffer(currentFrame);

	commandBuffers[currentFrame].reset();

	recordCommandBuffer(imageIndex, swapchainResources);

	std::array waitSemaphores{imageAvailableSemaphores[currentFrame].get()};
	std::array waitStages{vk::PipelineStageFlags{vk::PipelineStageFlagBits::eColorAttachmentOutput}};
	std::array signalSemaphores{renderFinishedSemaphores[currentFrame].get()};
	vk::SubmitInfo submitInfo{waitSemaphores, waitStages, commandBuffers[currentFrame], signalSemaphores};

	device->resetFences(inFlightFences[currentFrame].get());
	errorFatal(graphicsQueue.submit(submitInfo, inFlightFences[currentFrame].get()), "couldn't submit to queue"s);

	vk::PresentInfoKHR presentInfo{signalSemaphores, swapchainResources.swapchain.get(), imageIndex};

	vk::Result presentResult{};
	presentResult = presentationQueue.presentKHR(presentInfo);
	if (!isSwapchainRetired)
	{
		if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || framebufferResized)
		{
			framebufferResized = false;
			recreateSwapchainResources();
		}
		else if (presentResult != vk::Result::eSuccess)
		{
			errorFatal(presentResult, "couldn't present image"s);
		}
	}
}

void VulkanResources::toggleWireframeMode()
{
	if (supportedFeatures.fillModeNonSolid)
	{
		if (swapchainResources->graphicsPipelines.type() == RenderingPipelines::Type::Wireframe)
		{
			swapchainResources->graphicsPipelines.switchPipeline(RenderingPipelines::Type::Main);
		}
		else
		{
			swapchainResources->graphicsPipelines.switchPipeline(RenderingPipelines::Type::Wireframe);
		}
	}
}