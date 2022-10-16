#pragma once

#include <unordered_set>

#include "print.h"
#include "helpers.h"
#include "constants.h"
#include "Window.h"
#include "logging.h"



class VulkanResources
{
public:
	VulkanResources();
	~VulkanResources();

	bool windowCloseStatus();
	void drawFrame();
	void stopRendering();
private:
	WindowContext windowContext;
	Window renderWindow;
	vk::UniqueInstance instance;
	vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger;
	vk::UniqueSurfaceKHR surface;
	vk::PhysicalDevice physicalDevice;
	QueueFamilyIndices queueFamilyIndices;
	vk::UniqueDevice device;
	vk::Queue graphicsQueue;
	vk::Queue presentationQueue;
	vk::UniqueSwapchainKHR swapchain;
	std::vector<vk::Image> swapchainImages;
	vk::Format swapchainImageFormat;
	vk::Extent2D swapchainExtent;
	std::vector<vk::UniqueImageView> swapchainImageViews;
	vk::UniqueRenderPass renderPass;
	vk::UniquePipelineLayout pipelineLayout;
	vk::UniquePipeline graphicsPipeline;
	std::vector<vk::UniqueFramebuffer> swapchainFramebuffers;
	vk::UniqueCommandPool commandPool;
	std::vector<vk::CommandBuffer> commandBuffers;
	std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
	std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
	std::vector<vk::UniqueFence> inFlightFences;
	uint64_t currentFrame{0};
};

