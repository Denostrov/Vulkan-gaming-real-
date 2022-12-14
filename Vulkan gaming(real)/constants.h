#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_ASSERT_ON_RESULT ignore
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

using namespace std::literals;

static constexpr std::array<char const*, 1> VALIDATION_LAYERS{"VK_LAYER_KHRONOS_validation"};
static constexpr std::array<char const*, 1> DEVICE_EXTENSIONS{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
static constexpr std::array<char const*, 1> OPTIONAL_DEVICE_FEATURES{ };

static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
static constexpr double TIME_STEP = 0.0078125;

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;

	static auto getBindingDescription()
	{
		vk::VertexInputBindingDescription bindingDescription{0, sizeof(Vertex), vk::VertexInputRate::eVertex};
		return bindingDescription;
	}
	static auto getAttributeDescriptions()
	{
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{
			{0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position)},
			{1, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, texCoord)}
		};
		return attributeDescriptions;
	}
};

struct InstanceVertex
{
	glm::vec4 color;
	glm::vec3 position;
	glm::vec2 scale;
	glm::vec4 texOffsetScale;

	static auto getBindingDescription()
	{
		vk::VertexInputBindingDescription bindingDescription{1, sizeof(InstanceVertex), vk::VertexInputRate::eInstance};
		return bindingDescription;
	}
	static auto getAttributeDescriptions()
	{
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{
			{2, 1, vk::Format::eR32G32B32A32Sfloat, offsetof(InstanceVertex, color)},
			{3, 1, vk::Format::eR32G32B32Sfloat, offsetof(InstanceVertex, position)},
			{4, 1, vk::Format::eR32G32Sfloat, offsetof(InstanceVertex, scale)},
			{5, 1, vk::Format::eR32G32B32A32Sfloat, offsetof(InstanceVertex, texOffsetScale)}
		};
		return attributeDescriptions;
	}
};

struct UniformBufferObject
{
	glm::mat4 vp;
};

static constexpr std::array<Vertex, 4> vertices = {
	Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	Vertex{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	Vertex{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
	Vertex{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}
};

static constexpr std::array<uint16_t, 4> indices = {0, 1, 2, 3};

#ifdef NDEBUG
static constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif