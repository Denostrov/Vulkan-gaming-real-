#pragma once

#include <string>

struct QueueFamilyIndices
{
	uint32_t graphicsFamily;
	uint32_t presentationFamily;
};

struct SwapChainSupportDetails
{
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

template<class Vec, class Func>
inline bool checkVectorContainsString(Vec vector, Func vecStrAccess, char const* str)
{
	return std::find_if(vector.begin(), vector.end(),
		[&](auto const& p) { return std::strcmp(vecStrAccess(p), str); }) != vector.end();
}