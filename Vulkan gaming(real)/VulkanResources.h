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
	vk::Format swapChainImageFormat;
	vk::Extent2D swapChainExtent;

	vk::UniqueInstance createInstance(std::vector<char const*> const& validationLayers, std::vector<char const*> const& instanceExtensions,
									  vk::DebugUtilsMessengerCreateInfoEXT* debugUtilsMessengerCreateInfo);
	vk::UniqueDebugUtilsMessengerEXT createDebugUtilsMessenger(vk::DebugUtilsMessengerCreateInfoEXT const& debugUtilsMessengerCreateInfo);
	vk::UniqueSurfaceKHR createSurface();
	vk::UniqueDevice createDevice(std::vector<char const*> const& validationLayers, std::vector<char const*> const& requiredPhysicalDeviceExtensions);
	std::tuple<vk::UniqueSwapchainKHR, std::vector<vk::Image>, vk::Format, vk::Extent2D> createSwapchain(SwapChainSupportDetails const& swapChainSupportDetails);
};

