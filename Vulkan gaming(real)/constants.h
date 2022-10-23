#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_ASSERT_ON_RESULT ignore
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <array>

using namespace std::literals;

static constexpr std::array<char const*, 1> VALIDATION_LAYERS{"VK_LAYER_KHRONOS_validation"};
static constexpr std::array<char const*, 1> DEVICE_EXTENSIONS{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
static constexpr std::array<char const*, 1> OPTIONAL_DEVICE_FEATURES{ };

static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

struct Vertex
{
	glm::vec2 position;
	glm::vec3 color;

	static auto getBindingDescription()
	{
		vk::VertexInputBindingDescription bindingDescription{0, sizeof(Vertex), vk::VertexInputRate::eVertex};
		return bindingDescription;
	}
	static auto getAttributeDescriptions()
	{
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{
			vk::VertexInputAttributeDescription{0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)},
			vk::VertexInputAttributeDescription{1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)}
		};
		return attributeDescriptions;
	}
};

static constexpr std::array<Vertex, 4> vertices = {
	Vertex{{-0.5f, -0.5f},	{1.0f, 0.0f, 0.0f}},
	Vertex{{0.5f, -0.5f},	{0.0f, 1.0f, 0.0f}},
	Vertex{{-0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}},
	Vertex{{0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}}
};

static constexpr std::array<uint16_t, 4> indices = {0, 1, 2, 3};

#ifdef NDEBUG
static constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif