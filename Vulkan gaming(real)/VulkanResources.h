#pragma once

#include "constants.h"
#include "Window.h"

class VulkanResources;

struct QueueFamilyIndices
{
	uint32_t graphicsFamily;
	uint32_t presentationFamily;
};

struct SwapchainSupportDetails
{
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

struct RenderingPipelines
{
	enum class Type : uint64_t
	{
		Main = 0, Wireframe = 1
	};

	RenderingPipelines() = default;
	RenderingPipelines(VulkanResources& vulkan, vk::Extent2D viewportExtent, vk::RenderPass renderPass, Type initialType);
	vk::Pipeline current() const { return pipelines[currentIndex].get(); }
	Type type() const { return static_cast<Type>(currentIndex); }
	std::size_t size() const { return pipelines.size(); }
	void switchPipeline(Type type) { assert(pipelines[std::to_underlying(type)]); currentIndex = std::to_underlying(type); }

private:
	uint64_t currentIndex{0};
	std::vector<vk::UniquePipeline> pipelines;
};

struct SwapchainResources
{
	SwapchainResources(VulkanResources& vulkan, RenderingPipelines::Type initialType, vk::SwapchainKHR oldSwapchain = nullptr);

	vk::UniqueSwapchainKHR swapchain;
	std::vector<vk::Image> swapchainImages;
	vk::Format swapchainImageFormat;
	vk::Extent2D swapchainExtent;
	std::vector<vk::UniqueImageView> swapchainImageViews;
	vk::UniqueRenderPass renderPass;
	std::vector<vk::UniqueFramebuffer> swapchainFramebuffers;
	RenderingPipelines graphicsPipelines;
};

template<class T>
struct OldResourceQueue
{
	struct OldResource
	{
		std::unique_ptr<T> resource;
		uint64_t conditionsLeft;
	};
	void addToCleanup(std::unique_ptr<T>&& res, uint64_t waitCount);
	void updateCleanup();
	T& operator[](uint64_t index) { return *oldResources[index].resource; }
	T const& operator[](uint64_t index) const { return *oldResources[index].resource; }
	uint64_t size() const { return oldResources.size(); }

private:
	std::vector<OldResource> oldResources;
};

class VulkanResources
{
public:
	VulkanResources();

	bool windowCloseStatus();
	void drawFrame();
	void stopRendering();
	void toggleWireframeMode();

	bool framebufferResized = false;
private:
	WindowContext windowContext;
	Window renderWindow;
	vk::UniqueInstance instance;
	vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger;
	vk::UniqueSurfaceKHR surface;
	vk::PhysicalDevice physicalDevice;
	vk::PhysicalDeviceFeatures supportedFeatures;
	QueueFamilyIndices queueFamilyIndices;
	vk::UniqueDevice device;
	vk::Queue graphicsQueue;
	vk::Queue presentationQueue;
	vk::UniqueDescriptorSetLayout descriptorSetLayout;
	vk::UniquePipelineLayout pipelineLayout;
	std::unique_ptr<SwapchainResources> swapchainResources;
	OldResourceQueue<SwapchainResources> oldSwapchainResources;
	vk::UniqueCommandPool commandPool;
	vk::UniqueCommandPool shortBufferCommandPool;
	vk::UniqueBuffer vertexBuffer;
	vk::UniqueDeviceMemory vertexBufferMemory;
	vk::UniqueBuffer indexBuffer;
	vk::UniqueDeviceMemory indexBufferMemory;
	std::vector<vk::UniqueBuffer> uniformBuffers;
	std::vector<vk::UniqueDeviceMemory> uniformBuffersMemory;
	vk::UniqueDescriptorPool descriptorPool;
	std::vector<vk::DescriptorSet> descriptorSets;
	std::vector<vk::CommandBuffer> commandBuffers;
	std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
	std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
	std::vector<vk::UniqueFence> inFlightFences;
	uint64_t currentFrame{0};

	auto createDebugUtilsMessenger(vk::DebugUtilsMessengerCreateInfoEXT const& debugUtilsMessengerCreateInfo);
	auto createSurface();
	auto getSwapchainSupportDetails(vk::PhysicalDevice const& physicalDevice);
	auto getQueueFamilyIndices(vk::PhysicalDevice physicalDevice);
	auto rateDeviceScore(vk::PhysicalDevice const& physicalDevice, std::vector<char const*> const& requiredPhysicalDeviceExtensions);
	auto choosePhysicalDevice(std::vector<char const*> const& requiredPhysicalDeviceExtensions);
	auto createDevice(std::vector<char const*> const& validationLayers, std::vector<char const*> const& requiredPhysicalDeviceExtensions);
	auto createSwapchain(SwapchainSupportDetails const& swapchainSupportDetails, vk::SwapchainKHR oldSwapchain = nullptr);
	auto createSwapchainImageViews(std::vector<vk::Image> const& swapchainImages, vk::Format swapchainImageFormat);
	auto createRenderPass(vk::Format swapchainImageFormat);
	auto createFramebuffers(std::vector<vk::UniqueImageView> const& swapchainImageViews, vk::Extent2D const& swapchainExtent, vk::RenderPass renderPass);
	auto createShaderModule(std::vector<char> const& shaderCode);
	auto createDescriptorSetLayout();
	auto createGraphicsPipelineLayout();
	auto createGraphicsPipeline(vk::Extent2D viewportExtent, vk::RenderPass renderPass, vk::PolygonMode polygonMode);
	auto createCommandPool(vk::CommandPoolCreateFlags flags);
	auto createBuffer(vk::DeviceSize size, vk::BufferUsageFlags bufferUsage, vk::MemoryPropertyFlags memoryProperties);
	auto copyBuffer(vk::Buffer sourceBuffer, vk::Buffer destBuffer, vk::DeviceSize size);
	auto createUniformBuffers();
	auto createDescriptorPool();
	auto createDescriptorSets();

	template<class Data>
	auto createDeviceLocalBuffer(Data const& data, vk::BufferUsageFlags bufferUsage);

	auto createCommandBuffers();
	auto updateUniformBuffer(uint64_t frameIndex);
	auto recordCommandBuffer(uint32_t imageIndex, SwapchainResources const& swapchainResources, vk::Buffer vertexBuffer);
	auto createSyncObjects();
	void submitImage(SwapchainResources const& swapchain, uint32_t imageIndex, bool isSwapchainRetired = false);
	void recreateSwapchainResources();

	friend struct SwapchainResources;
	friend struct RenderingPipelines;
};

