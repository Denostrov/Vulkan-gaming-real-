#pragma once

#include "constants.h"
#include "Window.h"

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

struct SwapchainResources
{
	SwapchainResources() = default;
	SwapchainResources(vk::Device device, vk::SurfaceKHR surface, GLFWwindow* window, QueueFamilyIndices const& queueFamilyIndices,
					   SwapchainSupportDetails const& swapchainSupportDetails, vk::SwapchainKHR oldSwapchain);

	vk::UniqueSwapchainKHR swapchain;
	std::vector<vk::Image> swapchainImages;
	vk::Format swapchainImageFormat;
	vk::Extent2D swapchainExtent;
	std::vector<vk::UniqueImageView> swapchainImageViews;
	vk::UniqueRenderPass renderPass;
	std::vector<vk::UniqueFramebuffer> swapchainFramebuffers;
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
	void recreateSwapchainResources();
	void submitOldImage(SwapchainResources const& oldSwapchain, uint32_t imageIndex);

	bool framebufferResized = false;
private:
	WindowContext windowContext;
	Window renderWindow;
	vk::UniqueInstance instance;
	vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger;
	vk::UniqueSurfaceKHR surface;
	vk::PhysicalDevice physicalDevice;
	QueueFamilyIndices queueFamilyIndices;
	SwapchainSupportDetails swapchainSupportDetails;
	vk::UniqueDevice device;
	vk::Queue graphicsQueue;
	vk::Queue presentationQueue;
	std::unique_ptr<SwapchainResources> swapchainResources;
	OldResourceQueue<SwapchainResources> oldSwapchainResources;
	vk::UniquePipelineLayout pipelineLayout;
	vk::UniquePipeline graphicsPipeline;
	vk::UniqueCommandPool commandPool;
	std::vector<vk::CommandBuffer> commandBuffers;
	std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
	std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
	std::vector<vk::UniqueFence> inFlightFences;
	uint64_t currentFrame{0};
};

