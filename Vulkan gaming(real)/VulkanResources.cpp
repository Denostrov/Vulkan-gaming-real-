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
		assert(checkAndPrintRequired(availableLayers, requiredLayers) && "Not all validation layers supported");

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
	assert(checkAndPrintRequired(availableInstanceExtensions, requiredInstanceExtensions) && "Not all instance extensions available");

	return requiredInstanceExtensions;
}

auto getQueueFamilyIndices(vk::PhysicalDevice const& physicalDevice, vk::SurfaceKHR const& surface)
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
	score += physicalDeviceProperties.limits.maxFramebufferHeight;
	score += physicalDeviceProperties.limits.maxImageArrayLayers;
	score += physicalDeviceProperties.limits.maxVertexInputBindings;

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

	return std::make_tuple(score * multiplier, queueFamilyIndices, swapChainSupportDetails);
}

auto choosePhysicalDevice(vk::Instance const& instance, std::vector<char const*> const& requiredPhysicalDeviceExtensions, vk::SurfaceKHR const& surface)
{
	auto physicalDevices = instance.enumeratePhysicalDevices();
	assert(!physicalDevices.empty() && "No devices with vulkan support found");

	uint64_t maxPhysicalDeviceScore = 0;
	vk::PhysicalDevice bestPhysicalDevice{};
	QueueFamilyIndices bestPhysicalDeviceQueueIndices{};
	SwapChainSupportDetails bestSwapChainSupportDetails{};
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
			bestSwapChainSupportDetails = currentSwapChainSupportDetails;
			maxPhysicalDeviceScore = std::max(maxPhysicalDeviceScore, currentPhysicalDeviceScore);
		}
	}
	assert(maxPhysicalDeviceScore > 0 && "No suitable physical devices found");
	formatPrint(std::cout, "Picked {} as best physical device with {} score and {} queue family indices\n"sv,
				bestPhysicalDevice.getProperties().deviceName.data(), maxPhysicalDeviceScore, toString(bestPhysicalDeviceQueueIndices));
	return std::make_tuple(bestPhysicalDevice, bestPhysicalDeviceQueueIndices, bestSwapChainSupportDetails);
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
	for (auto const& mode : presentModes)
	{
		if (mode == vk::PresentModeKHR::eMailbox)
		{
			chosenPresentMode = mode;
			break;
		}
	}
	chosenPresentMode = vk::PresentModeKHR::eFifo;
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

auto createDebugUtilsMessenger(vk::Instance instance, vk::DebugUtilsMessengerCreateInfoEXT const& debugUtilsMessengerCreateInfo)
{
	return instance.createDebugUtilsMessengerEXTUnique(debugUtilsMessengerCreateInfo);
}

auto createSurface(vk::Instance instance, GLFWwindow* window)
{
	VkSurfaceKHR windowSurface;
	glfwCreateWindowSurface(instance, window, nullptr, &windowSurface);
	return vk::UniqueSurfaceKHR(vk::SurfaceKHR(windowSurface), instance);
}

auto createDevice(vk::PhysicalDevice physicalDevice, QueueFamilyIndices const& queueFamilyIndices, std::vector<char const*> const& validationLayers,
				  std::vector<char const*> const& requiredPhysicalDeviceExtensions)
{
	std::vector<float> queuePriorities{1.0f};
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

auto createSwapchain(vk::Device device, vk::SurfaceKHR surface, GLFWwindow* window, QueueFamilyIndices const& queueFamilyIndices,
					 SwapChainSupportDetails const& swapChainSupportDetails)
{
	auto surfaceFormat = chooseSwapSurfaceFormat(swapChainSupportDetails.formats);
	auto presentMode = chooseSwapPresentMode(swapChainSupportDetails.presentModes);
	auto extent = chooseSwapExtent(window, swapChainSupportDetails.capabilities);

	uint32_t imageCount = swapChainSupportDetails.capabilities.minImageCount + 1;
	if (swapChainSupportDetails.capabilities.maxImageCount > 0 && imageCount > swapChainSupportDetails.capabilities.maxImageCount)
	{
		imageCount = swapChainSupportDetails.capabilities.maxImageCount;
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

	vk::SwapchainCreateInfoKHR createInfo{{}, surface, imageCount, surfaceFormat.format, surfaceFormat.colorSpace, extent, 1,
		vk::ImageUsageFlagBits::eColorAttachment, imageSharingMode, queueIndices, swapChainSupportDetails.capabilities.currentTransform,
		vk::CompositeAlphaFlagBitsKHR::eOpaque, presentMode, vk::Bool32(true)};

	auto newSwapchain = device.createSwapchainKHRUnique(createInfo);
	auto newSwapChainImages = device.getSwapchainImagesKHR(newSwapchain.get());
	assert(!newSwapChainImages.empty() && "couldn't get swapchain images");
	return std::make_tuple(std::move(newSwapchain), newSwapChainImages, surfaceFormat.format, extent);
}

auto createSwapchainImageViews(vk::Device device, std::vector<vk::Image> const& swapchainImages, vk::Format swapchainImageFormat)
{
	std::vector<vk::UniqueImageView> result{};
	for (auto const& image : swapchainImages)
	{
		vk::ImageSubresourceRange subresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};
		vk::ImageViewCreateInfo imageViewCreateInfo({}, image, vk::ImageViewType::e2D, swapchainImageFormat,
													{vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,vk::ComponentSwizzle::eIdentity,vk::ComponentSwizzle::eIdentity},
													subresourceRange);
		result.push_back(device.createImageViewUnique(imageViewCreateInfo));
	}

	return result;
}

auto createShaderModule(vk::Device device, std::vector<char> const& shaderCode)
{
	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{{}, shaderCode.size(), reinterpret_cast<uint32_t const*>(shaderCode.data())};

	return device.createShaderModuleUnique(shaderModuleCreateInfo);
}

auto createRenderPass(vk::Device device, vk::Format swapchainImageFormat)
{
	vk::AttachmentDescription colorAttachment{{}, swapchainImageFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
	vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR};

	vk::AttachmentReference colorAttachmentRef{0, vk::ImageLayout::eColorAttachmentOptimal};
	vk::SubpassDescription subpass{{}, vk::PipelineBindPoint::eGraphics, nullptr, colorAttachmentRef};

	vk::SubpassDependency subpassDependency{VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput,
											vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::AccessFlagBits{0}, vk::AccessFlagBits::eColorAttachmentWrite};

	vk::RenderPassCreateInfo renderPassCreateInfo{{}, colorAttachment, subpass, subpassDependency};
	return device.createRenderPassUnique(renderPassCreateInfo);
}

auto createGraphicsPipeline(vk::Device device, vk::Extent2D const& swapchainExtent, vk::RenderPass renderPass)
{
	auto vertexShaderCode = readFile("shaders/vertex.spv");
	auto fragmentShaderCode = readFile("shaders/fragment.spv");
	assert(!vertexShaderCode.empty() && !fragmentShaderCode.empty() && "couldn't read shader files");

	vk::UniqueShaderModule vertexShaderModule = createShaderModule(device, vertexShaderCode);
	formatPrint(std::cout, "Created vertex shader module\n"sv);
	vk::UniqueShaderModule fragmentShaderModule = createShaderModule(device, fragmentShaderCode);
	formatPrint(std::cout, "Created fragment shader module\n"sv);

	vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo{{}, vk::ShaderStageFlagBits::eVertex, vertexShaderModule.get(), "main"};
	vk::PipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{{}, vk::ShaderStageFlagBits::eFragment, fragmentShaderModule.get(), "main"};

	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages{vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo};

	std::vector<vk::DynamicState> dynamicStates{vk::DynamicState::eViewport, vk::DynamicState::eScissor};

	vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo{{}, dynamicStates};

	std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions{};
	std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions{};
	vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{{}, vertexInputBindingDescriptions, vertexInputAttributeDescriptions};

	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{{}, vk::PrimitiveTopology::eTriangleStrip, VK_FALSE};

	vk::Viewport viewport{0.0f, 0.0f, static_cast<float>(swapchainExtent.width), static_cast<float>(swapchainExtent.height), 0.0f, 1.0f};
	vk::Rect2D scissor{{0, 0}, swapchainExtent};

	vk::PipelineViewportStateCreateInfo viewportStateCreateInfo{{}, viewport, scissor};

	vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{{}, VK_FALSE, VK_FALSE, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack,
		vk::FrontFace::eClockwise, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f};

	vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo{{}, vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr, VK_FALSE, VK_FALSE};

	vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{VK_FALSE, vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
		vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
		vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};

	vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{{}, VK_FALSE, vk::LogicOp::eCopy, colorBlendAttachmentState, {0.0f, 0.0f, 0.0f, 0.0f}};

	vk::PipelineLayoutCreateInfo layoutCreateInfo{{}, nullptr, nullptr};
	auto pipelineLayout = device.createPipelineLayoutUnique(layoutCreateInfo);

	vk::GraphicsPipelineCreateInfo pipelineCreateInfo{{}, shaderStages, &vertexInputStateCreateInfo, &inputAssemblyStateCreateInfo, nullptr,
		&viewportStateCreateInfo, &rasterizationStateCreateInfo, &multisampleStateCreateInfo, nullptr, &colorBlendStateCreateInfo, &dynamicStateCreateInfo,
		pipelineLayout.get(), renderPass, 0};
	auto [result, pipeline] = device.createGraphicsPipelineUnique(nullptr, pipelineCreateInfo);
	assert(result == vk::Result::eSuccess && "couldn't create graphics pipeline");
	return std::make_tuple(std::move(pipelineLayout), std::move(pipeline));
}

auto createFramebuffers(vk::Device device, std::vector<vk::UniqueImageView> const& swapchainImageViews, vk::Extent2D const& swapchainExtent,
						vk::RenderPass renderPass)
{
	std::vector<vk::UniqueFramebuffer> framebuffers;

	for (size_t i = 0; i < swapchainImageViews.size(); i++)
	{
		vk::FramebufferCreateInfo framebufferCreateInfo{{}, renderPass, swapchainImageViews[i].get(), swapchainExtent.width, swapchainExtent.height, 1};
		framebuffers.push_back(device.createFramebufferUnique(framebufferCreateInfo));
	}
	return framebuffers;
}

auto createCommandPool(vk::Device device, QueueFamilyIndices const& queueFamilyIndices)
{
	vk::CommandPoolCreateInfo commandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamilyIndices.graphicsFamily};
	return device.createCommandPoolUnique(commandPoolCreateInfo);
}

auto createCommandBuffers(vk::Device device, vk::CommandPool commandPool)
{
	vk::CommandBufferAllocateInfo commandBufferAllocateInfo{commandPool, vk::CommandBufferLevel::ePrimary, MAX_FRAMES_IN_FLIGHT};
	return device.allocateCommandBuffers(commandBufferAllocateInfo);
}

auto recordCommandBuffer(vk::CommandBuffer commandBuffer, vk::Pipeline graphicsPipeline, vk::RenderPass renderPass, vk::Framebuffer swapchainFrameBuffer,
						 vk::Extent2D const& swapchainExtent)
{
	vk::CommandBufferBeginInfo commandBufferBeginInfo{{}, nullptr};
	commandBuffer.begin(commandBufferBeginInfo);

	vk::ClearValue clearColor{vk::ClearColorValue{std::array{0.0f, 0.0f, 0.0f, 1.0f}}};
	vk::RenderPassBeginInfo renderPassBeginInfo{renderPass, swapchainFrameBuffer, {{0, 0}, swapchainExtent}, clearColor};

	commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

	vk::Viewport viewport{0.0f, 0.0f, static_cast<float>(swapchainExtent.width), static_cast<float>(swapchainExtent.height), 0.0f, 1.0f};
	commandBuffer.setViewport(0, viewport);

	vk::Rect2D scissor{{0, 0}, swapchainExtent};
	commandBuffer.setScissor(0, scissor);

	commandBuffer.draw(3, 1, 0, 0);

	commandBuffer.endRenderPass();

	commandBuffer.end();
}

auto createSyncObjects(vk::Device device)
{
	vk::SemaphoreCreateInfo semaphoreCreateInfo{{}, nullptr};
	vk::FenceCreateInfo fenceCreateInfo{vk::FenceCreateFlagBits::eSignaled, nullptr};

	std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
	std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
	std::vector<vk::UniqueFence> inFlightFences;
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		imageAvailableSemaphores.push_back(device.createSemaphoreUnique(semaphoreCreateInfo));
		renderFinishedSemaphores.push_back(device.createSemaphoreUnique(semaphoreCreateInfo));
		inFlightFences.push_back(device.createFenceUnique(fenceCreateInfo));
	}
	return std::make_tuple(std::move(imageAvailableSemaphores), std::move(renderFinishedSemaphores), std::move(inFlightFences));
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
	formatPrint(std::cout, "Created an instance\n"sv);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.get());

	if (ENABLE_VALIDATION_LAYERS)
	{
		debugUtilsMessenger = createDebugUtilsMessenger(instance.get(), *debugUtilsMessengerCreateInfo);
		formatPrint(std::cout, "Created a debug messenger\n"sv);
	}

	surface = createSurface(instance.get(), renderWindow);
	formatPrint(std::cout, "Created a window surface\n"sv);

	SwapChainSupportDetails swapchainSupportDetails;
	std::tie(physicalDevice, queueFamilyIndices, swapchainSupportDetails) = choosePhysicalDevice(instance.get(), requiredPhysicalDeviceExtensions, surface.get());

	device = createDevice(physicalDevice, queueFamilyIndices, validationLayers, requiredPhysicalDeviceExtensions);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(device.get());
	formatPrint(std::cout, "Created logical device\n"sv);

	graphicsQueue = device->getQueue(queueFamilyIndices.graphicsFamily, 0);
	formatPrint(std::cout, "Acquired graphics queue\n"sv);
	presentationQueue = device->getQueue(queueFamilyIndices.presentationFamily, 0);
	formatPrint(std::cout, "Acquired presentation queue\n"sv);

	std::tie(swapchain, swapchainImages, swapchainImageFormat, swapchainExtent) = createSwapchain(device.get(), surface.get(), renderWindow, queueFamilyIndices,
																								  swapchainSupportDetails);
	formatPrint(std::cout, "Created swapchain\n"sv);
	formatPrint(std::cout, "{} swapchain images acquired\n"sv, swapchainImages.size());

	swapchainImageViews = createSwapchainImageViews(device.get(), swapchainImages, swapchainImageFormat);
	formatPrint(std::cout, "Created {} swapchain image views\n"sv, swapchainImageViews.size());

	renderPass = createRenderPass(device.get(), swapchainImageFormat);
	formatPrint(std::cout, "Created renderpass\n"sv);

	std::tie(pipelineLayout, graphicsPipeline) = createGraphicsPipeline(device.get(), swapchainExtent, renderPass.get());
	formatPrint(std::cout, "Created graphics pipeline\n"sv);

	swapchainFramebuffers = createFramebuffers(device.get(), swapchainImageViews, swapchainExtent, renderPass.get());
	formatPrint(std::cout, "Created {} framebuffers\n"sv, swapchainFramebuffers.size());

	commandPool = createCommandPool(device.get(), queueFamilyIndices);
	formatPrint(std::cout, "Created command pool\n"sv);

	commandBuffers = createCommandBuffers(device.get(), commandPool.get());
	formatPrint(std::cout, "Allocated command buffer\n"sv);

	std::tie(imageAvailableSemaphores, renderFinishedSemaphores, inFlightFences) = createSyncObjects(device.get());
	formatPrint(std::cout, "Created synchronization resources\n"sv);
}

VulkanResources::~VulkanResources()
{

}

bool VulkanResources::windowCloseStatus()
{
	return glfwWindowShouldClose(renderWindow);
}

void VulkanResources::drawFrame()
{
	auto waitResult = device->waitForFences(inFlightFences[currentFrame].get(), VK_TRUE, std::numeric_limits<uint64_t>::max());
	device->resetFences(inFlightFences[currentFrame].get());

	auto [acquireResult, imageIndex] = device->acquireNextImageKHR(swapchain.get(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame].get());

	commandBuffers[currentFrame].reset();

	recordCommandBuffer(commandBuffers[currentFrame], graphicsPipeline.get(), renderPass.get(), swapchainFramebuffers[imageIndex].get(), swapchainExtent);

	std::array waitSemaphores{imageAvailableSemaphores[currentFrame].get()};
	std::array waitStages{vk::PipelineStageFlags{vk::PipelineStageFlagBits::eColorAttachmentOutput}};
	std::array signalSemaphores{renderFinishedSemaphores[currentFrame].get()};
	vk::SubmitInfo submitInfo{waitSemaphores, waitStages, commandBuffers[currentFrame], signalSemaphores};

	graphicsQueue.submit(submitInfo, inFlightFences[currentFrame].get());

	vk::PresentInfoKHR presentInfo{signalSemaphores, swapchain.get(), imageIndex};

	auto presentResult = presentationQueue.presentKHR(presentInfo);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanResources::stopRendering()
{
	device->waitIdle();
}