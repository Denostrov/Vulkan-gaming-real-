#pragma once

#include <iostream>
#include <format>
#include <string>
#include <string_view>

#include "constants.h"
#include "VulkanResources.h"

using namespace std::literals;

template<class T>
concept Number = std::is_integral_v<T> || std::is_floating_point_v<T>;
template<Number T>
inline std::string toString(T val)
{
	return std::to_string(val);
}
template<std::size_t N>
inline std::string toString(vk::ArrayWrapper1D<char, N> const& arr)
{
	return arr;
}
template<class T, std::size_t N>
inline std::string toString(std::array<T, N> const& arr)
{
	auto result = "{"s;
	for (auto it = arr.begin(); it != arr.end(); it++)
	{
		result += toString(*it);
		if (it + 1 != arr.end()) result += ","s;
	}
	result += "}"s;
	return result;
}
inline std::string toString(vk::PhysicalDeviceType type)
{
	switch (type)
	{
	case vk::PhysicalDeviceType::eOther:
		return "Other"s;
		break;
	case vk::PhysicalDeviceType::eIntegratedGpu:
		return "Integrated GPU"s;
		break;
	case vk::PhysicalDeviceType::eDiscreteGpu:
		return "Discrete GPU"s;
		break;
	case vk::PhysicalDeviceType::eVirtualGpu:
		return "Virtual GPU"s;
		break;
	case vk::PhysicalDeviceType::eCpu:
		return "CPU"s;
		break;
	default:
		return "Unknown Physical Device Type"s;
		break;
	}
}
inline std::string toString(vk::SampleCountFlags flags)
{
	return toString((VkSampleCountFlags(flags) + 1) / 2);
}
inline std::string toString(bool val)
{
	return val ? "True"s : "False"s;
}
inline std::string toString(QueueFamilyIndices const& val)
{
	return std::vformat("{{Graphics: {}, Presentation: {}}}"sv, std::make_format_args(val.graphicsFamily, val.presentationFamily));
}
inline std::string toString(vk::Extent2D const& val)
{
	return std::vformat("{{{},{}}}"sv, std::make_format_args(val.width, val.height));
}
inline std::string toString(vk::Extent3D const& val)
{
	return std::vformat("{{{},{},{}}}"sv, std::make_format_args(val.width, val.height, val.depth));
}
inline std::string toString(vk::SurfaceTransformFlagBitsKHR val)
{
	switch (val)
	{
	case vk::SurfaceTransformFlagBitsKHR::eIdentity:
		return "Identity"s;
		break;
	case vk::SurfaceTransformFlagBitsKHR::eRotate90:
		return "Rotate 90"s;
		break;
	case vk::SurfaceTransformFlagBitsKHR::eRotate180:
		return "Rotate 180"s;
		break;
	case vk::SurfaceTransformFlagBitsKHR::eRotate270:
		return "Rotate 270"s;
		break;
	case vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror:
		return "Horizontal mirror"s;
		break;
	case vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90:
		return "Horizontal mirror rotate 90"s;
		break;
	case vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180:
		return "Horizontal mirror rotate 180"s;
		break;
	case vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270:
		return "Horizontal mirror rotate 270"s;
		break;
	case vk::SurfaceTransformFlagBitsKHR::eInherit:
		return "Inherit"s;
		break;
	default:
		return "Unknown"s;
		break;
	}
}
inline std::string toString(vk::SurfaceTransformFlagsKHR val)
{
	auto result = ""s;
	result += val & vk::SurfaceTransformFlagBitsKHR::eIdentity ? toString(vk::SurfaceTransformFlagBitsKHR::eIdentity) + " "s : ""s;
	result += val & vk::SurfaceTransformFlagBitsKHR::eRotate90 ? toString(vk::SurfaceTransformFlagBitsKHR::eRotate90) + " "s : ""s;
	result += val & vk::SurfaceTransformFlagBitsKHR::eRotate180 ? toString(vk::SurfaceTransformFlagBitsKHR::eRotate180) + " "s : ""s;
	result += val & vk::SurfaceTransformFlagBitsKHR::eRotate270 ? toString(vk::SurfaceTransformFlagBitsKHR::eRotate270) + " "s : ""s;
	result += val & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror ? toString(vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror) + " "s : ""s;
	result += val & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90 ? toString(vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90) + " "s : ""s;
	result += val & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180 ? toString(vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180) + " "s : ""s;
	result += val & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270 ? toString(vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270) + " "s : ""s;
	result += val & vk::SurfaceTransformFlagBitsKHR::eInherit ? toString(vk::SurfaceTransformFlagBitsKHR::eInherit) : ""s;
	return result;
}
inline std::string toString(vk::CompositeAlphaFlagBitsKHR val)
{
	switch (val)
	{
	case vk::CompositeAlphaFlagBitsKHR::eOpaque:
		return "Opaque"s;
		break;
	case vk::CompositeAlphaFlagBitsKHR::ePreMultiplied:
		return "Pre Multiplied"s;
		break;
	case vk::CompositeAlphaFlagBitsKHR::ePostMultiplied:
		return "Post Multiplied"s;
		break;
	case vk::CompositeAlphaFlagBitsKHR::eInherit:
		return "Inherit"s;
		break;
	default:
		return "Unknown"s;
		break;
	}
}
inline std::string toString(vk::CompositeAlphaFlagsKHR val)
{
	auto result = ""s;
	result += val & vk::CompositeAlphaFlagBitsKHR::eOpaque ? toString(vk::CompositeAlphaFlagBitsKHR::eOpaque) + " "s : ""s;
	result += val & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied ? toString(vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) + " "s : ""s;
	result += val & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied ? toString(vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) + " "s : ""s;
	result += val & vk::CompositeAlphaFlagBitsKHR::eInherit ? toString(vk::CompositeAlphaFlagBitsKHR::eInherit) : ""s;
	return result;
}
inline std::string toString(vk::ImageUsageFlagBits val)
{
	switch (val)
	{
	case vk::ImageUsageFlagBits::eTransferSrc:
		return "Transfer source"s;
		break;
	case vk::ImageUsageFlagBits::eTransferDst:
		return "Transfer destination"s;
		break;
	case vk::ImageUsageFlagBits::eSampled:
		return "Sampled"s;
		break;
	case vk::ImageUsageFlagBits::eStorage:
		return "Storage"s;
		break;
	case vk::ImageUsageFlagBits::eColorAttachment:
		return "Color attachment"s;
		break;
	case vk::ImageUsageFlagBits::eDepthStencilAttachment:
		return "Depth stencil attachment"s;
		break;
	case vk::ImageUsageFlagBits::eTransientAttachment:
		return "Transient attachment"s;
		break;
	case vk::ImageUsageFlagBits::eInputAttachment:
		return "Input attachment"s;
		break;
	default:
		return "Unknown"s;
		break;
	}
}
inline std::string toString(vk::ImageUsageFlags val)
{
	auto result = ""s;
	result += val & vk::ImageUsageFlagBits::eTransferSrc ? toString(vk::ImageUsageFlagBits::eTransferSrc) + " "s : ""s;
	result += val & vk::ImageUsageFlagBits::eTransferDst ? toString(vk::ImageUsageFlagBits::eTransferDst) + " "s : ""s;
	result += val & vk::ImageUsageFlagBits::eSampled ? toString(vk::ImageUsageFlagBits::eSampled) + " "s : ""s;
	result += val & vk::ImageUsageFlagBits::eStorage ? toString(vk::ImageUsageFlagBits::eStorage) + " "s : ""s;
	result += val & vk::ImageUsageFlagBits::eColorAttachment ? toString(vk::ImageUsageFlagBits::eColorAttachment) + " "s : ""s;
	result += val & vk::ImageUsageFlagBits::eDepthStencilAttachment ? toString(vk::ImageUsageFlagBits::eDepthStencilAttachment) + " "s : ""s;
	result += val & vk::ImageUsageFlagBits::eTransientAttachment ? toString(vk::ImageUsageFlagBits::eTransientAttachment) + " "s : ""s;
	result += val & vk::ImageUsageFlagBits::eInputAttachment ? toString(vk::ImageUsageFlagBits::eInputAttachment) : ""s;
	return result;
}
inline std::string toString(vk::QueueFlagBits val)
{
	switch (val)
	{
	case vk::QueueFlagBits::eGraphics:
		return "Graphics"s;
		break;
	case vk::QueueFlagBits::eCompute:
		return "Compute"s;
		break;
	case vk::QueueFlagBits::eTransfer:
		return "Transfer"s;
		break;
	case vk::QueueFlagBits::eSparseBinding:
		return "Sparse binding"s;
		break;
	case vk::QueueFlagBits::eProtected:
		return "Protected"s;
		break;
	default:
		return "Unknown"s;
		break;
	}
}
inline std::string toString(vk::QueueFlags val)
{
	auto result = ""s;
	result += val & vk::QueueFlagBits::eGraphics ? toString(vk::QueueFlagBits::eGraphics) + " "s : ""s;
	result += val & vk::QueueFlagBits::eCompute ? toString(vk::QueueFlagBits::eCompute) + " "s : ""s;
	result += val & vk::QueueFlagBits::eTransfer ? toString(vk::QueueFlagBits::eTransfer) + " "s : ""s;
	result += val & vk::QueueFlagBits::eSparseBinding ? toString(vk::QueueFlagBits::eSparseBinding) + " "s : ""s;
	result += val & vk::QueueFlagBits::eProtected ? toString(vk::QueueFlagBits::eProtected) : ""s;
	return result;
}
inline std::string toString(vk::Format val)
{
	switch (val)
	{
	case vk::Format::eUndefined:
		return "Undefined"s;
		break;
	case vk::Format::eR4G4UnormPack8:
		return "eR4G4UnormPack8"s;
		break;
	case vk::Format::eR4G4B4A4UnormPack16:
		return "eR4G4B4A4UnormPack16"s;
		break;
	case vk::Format::eB4G4R4A4UnormPack16:
		return "eB4G4R4A4UnormPack16"s;
		break;
	case vk::Format::eR5G6B5UnormPack16:
		return "eR5G6B5UnormPack16"s;
		break;
	case vk::Format::eB5G6R5UnormPack16:
		return "eB5G6R5UnormPack16"s;
		break;
	case vk::Format::eR5G5B5A1UnormPack16:
		return "eR5G5B5A1UnormPack16"s;
		break;
	case vk::Format::eB5G5R5A1UnormPack16:
		return "eB5G5R5A1UnormPack16"s;
		break;
	case vk::Format::eA1R5G5B5UnormPack16:
		return "eA1R5G5B5UnormPack16"s;
		break;
	case vk::Format::eR8Unorm:
		return "eR8Unorm"s;
		break;
	case vk::Format::eR8Snorm:
		return "eR8Snorm"s;
		break;
	case vk::Format::eR8Uscaled:
		return "eR8Uscaled"s;
		break;
	case vk::Format::eR8Sscaled:
		return "eR8Sscaled"s;
		break;
	case vk::Format::eR8Uint:
		return "eR8Uint"s;
		break;
	case vk::Format::eR8Sint:
		return "eR8Sint"s;
		break;
	case vk::Format::eR8Srgb:
		return "eR8Srgb"s;
		break;
	case vk::Format::eR8G8Unorm:
		return "eR8G8Unorm"s;
		break;
	case vk::Format::eR8G8Snorm:
		return "eR8G8Snorm"s;
		break;
	case vk::Format::eR8G8Uscaled:
		return "eR8G8Uscaled"s;
		break;
	case vk::Format::eR8G8Sscaled:
		return "eR8G8Sscaled"s;
		break;
	case vk::Format::eR8G8Uint:
		return "eR8G8Uint"s;
		break;
	case vk::Format::eR8G8Sint:
		return "eR8G8Sint"s;
		break;
	case vk::Format::eR8G8Srgb:
		return "eR8G8Srgb"s;
		break;
	case vk::Format::eR8G8B8Unorm:
		return "eR8G8B8Unorm"s;
		break;
	case vk::Format::eR8G8B8Snorm:
		return "eR8G8B8Snorm"s;
		break;
	case vk::Format::eR8G8B8Uscaled:
		return "eR8G8B8Uscaled"s;
		break;
	case vk::Format::eR8G8B8Sscaled:
		return "eR8G8B8Sscaled"s;
		break;
	case vk::Format::eR8G8B8Uint:
		return "eR8G8B8Uint"s;
		break;
	case vk::Format::eR8G8B8Sint:
		return "eR8G8B8Sint"s;
		break;
	case vk::Format::eR8G8B8Srgb:
		return "eR8G8B8Srgb"s;
		break;
	case vk::Format::eB8G8R8Unorm:
		return "eB8G8R8Unorm"s;
		break;
	case vk::Format::eB8G8R8Snorm:
		return "eB8G8R8Snorm"s;
		break;
	case vk::Format::eB8G8R8Uscaled:
		return "eB8G8R8Uscaled"s;
		break;
	case vk::Format::eB8G8R8Sscaled:
		return "eB8G8R8Sscaled"s;
		break;
	case vk::Format::eB8G8R8Uint:
		return "eB8G8R8Uint"s;
		break;
	case vk::Format::eB8G8R8Sint:
		return "eB8G8R8Sint"s;
		break;
	case vk::Format::eB8G8R8Srgb:
		return "eB8G8R8Srgb"s;
		break;
	case vk::Format::eR8G8B8A8Unorm:
		return "eR8G8B8A8Unorm"s;
		break;
	case vk::Format::eR8G8B8A8Snorm:
		return "eR8G8B8A8Snorm"s;
		break;
	case vk::Format::eR8G8B8A8Uscaled:
		return "eR8G8B8A8Uscaled"s;
		break;
	case vk::Format::eR8G8B8A8Sscaled:
		return "eR8G8B8A8Sscaled"s;
		break;
	case vk::Format::eR8G8B8A8Uint:
		return "eR8G8B8A8Uint"s;
		break;
	case vk::Format::eR8G8B8A8Sint:
		return "eR8G8B8A8Sint"s;
		break;
	case vk::Format::eR8G8B8A8Srgb:
		return "eR8G8B8A8Srgb"s;
		break;
	case vk::Format::eB8G8R8A8Unorm:
		return "eB8G8R8A8Unorm"s;
		break;
	case vk::Format::eB8G8R8A8Snorm:
		return "eB8G8R8A8Snorm"s;
		break;
	case vk::Format::eB8G8R8A8Uscaled:
		return "eB8G8R8A8Uscaled"s;
		break;
	case vk::Format::eB8G8R8A8Sscaled:
		return "eB8G8R8A8Sscaled"s;
		break;
	case vk::Format::eB8G8R8A8Uint:
		return "eB8G8R8A8Uint"s;
		break;
	case vk::Format::eB8G8R8A8Sint:
		return "eB8G8R8A8Sint"s;
		break;
	case vk::Format::eB8G8R8A8Srgb:
		return "eB8G8R8A8Srgb"s;
		break;
	case vk::Format::eA8B8G8R8UnormPack32:
		return "eA8B8G8R8UnormPack32"s;
		break;
	case vk::Format::eA8B8G8R8SnormPack32:
		return "eA8B8G8R8SnormPack32"s;
		break;
	case vk::Format::eA8B8G8R8UscaledPack32:
		return "eA8B8G8R8UscaledPack32"s;
		break;
	case vk::Format::eA8B8G8R8SscaledPack32:
		return "eA8B8G8R8SscaledPack32"s;
		break;
	case vk::Format::eA8B8G8R8UintPack32:
		return "eA8B8G8R8UintPack32"s;
		break;
	case vk::Format::eA8B8G8R8SintPack32:
		return "eA8B8G8R8SintPack32"s;
		break;
	case vk::Format::eA8B8G8R8SrgbPack32:
		return "eA8B8G8R8SrgbPack32"s;
		break;
	case vk::Format::eA2R10G10B10UnormPack32:
		return "eA2R10G10B10UnormPack32"s;
		break;
	case vk::Format::eA2R10G10B10SnormPack32:
		return "eA2R10G10B10SnormPack32"s;
		break;
	case vk::Format::eA2R10G10B10UscaledPack32:
		return "eA2R10G10B10UscaledPack32"s;
		break;
	case vk::Format::eA2R10G10B10SscaledPack32:
		return "eA2R10G10B10SscaledPack32"s;
		break;
	case vk::Format::eA2R10G10B10UintPack32:
		return "eA2R10G10B10UintPack32"s;
		break;
	case vk::Format::eA2R10G10B10SintPack32:
		return "eA2R10G10B10SintPack32"s;
		break;
	case vk::Format::eA2B10G10R10UnormPack32:
		return "eA2B10G10R10UnormPack32"s;
		break;
	case vk::Format::eA2B10G10R10SnormPack32:
		return "eA2B10G10R10SnormPack32"s;
		break;
	case vk::Format::eA2B10G10R10UscaledPack32:
		return "eA2B10G10R10UscaledPack32"s;
		break;
	case vk::Format::eA2B10G10R10SscaledPack32:
		return "eA2B10G10R10SscaledPack32"s;
		break;
	case vk::Format::eA2B10G10R10UintPack32:
		return "eA2B10G10R10UintPack32"s;
		break;
	case vk::Format::eA2B10G10R10SintPack32:
		return "eA2B10G10R10SintPack32"s;
		break;
	case vk::Format::eR16Unorm:
		return "eR16Unorm"s;
		break;
	case vk::Format::eR16Snorm:
		return "eR16Snorm"s;
		break;
	case vk::Format::eR16Uscaled:
		return "eR16Uscaled"s;
		break;
	case vk::Format::eR16Sscaled:
		return "eR16Sscaled"s;
		break;
	case vk::Format::eR16Uint:
		return "eR16Uint"s;
		break;
	case vk::Format::eR16Sint:
		return "eR16Sint"s;
		break;
	case vk::Format::eR16Sfloat:
		return "eR16Sfloat"s;
		break;
	case vk::Format::eR16G16Unorm:
		return "eR16G16Unorm"s;
		break;
	case vk::Format::eR16G16Snorm:
		return "eR16G16Snorm"s;
		break;
	case vk::Format::eR16G16Uscaled:
		return "eR16G16Uscaled"s;
		break;
	case vk::Format::eR16G16Sscaled:
		return "eR16G16Sscaled"s;
		break;
	case vk::Format::eR16G16Uint:
		return "eR16G16Uint"s;
		break;
	case vk::Format::eR16G16Sint:
		return "eR16G16Sint"s;
		break;
	case vk::Format::eR16G16Sfloat:
		return "eR16G16Sfloat"s;
		break;
	case vk::Format::eR16G16B16Unorm:
		return "eR16G16B16Unorm"s;
		break;
	case vk::Format::eR16G16B16Snorm:
		return "eR16G16B16Snorm"s;
		break;
	case vk::Format::eR16G16B16Uscaled:
		return "eR16G16B16Uscaled"s;
		break;
	case vk::Format::eR16G16B16Sscaled:
		return "eR16G16B16Sscaled"s;
		break;
	case vk::Format::eR16G16B16Uint:
		return "eR16G16B16Uint"s;
		break;
	case vk::Format::eR16G16B16Sint:
		return "eR16G16B16Sint"s;
		break;
	case vk::Format::eR16G16B16Sfloat:
		return "eR16G16B16Sfloat"s;
		break;
	case vk::Format::eR16G16B16A16Unorm:
		return "eR16G16B16A16Unorm"s;
		break;
	case vk::Format::eR16G16B16A16Snorm:
		return "eR16G16B16A16Snorm"s;
		break;
	case vk::Format::eR16G16B16A16Uscaled:
		return "eR16G16B16A16Uscaled"s;
		break;
	case vk::Format::eR16G16B16A16Sscaled:
		return "eR16G16B16A16Sscaled"s;
		break;
	case vk::Format::eR16G16B16A16Uint:
		return "eR16G16B16A16Uint"s;
		break;
	case vk::Format::eR16G16B16A16Sint:
		return "eR16G16B16A16Sint"s;
		break;
	case vk::Format::eR16G16B16A16Sfloat:
		return "eR16G16B16A16Sfloat"s;
		break;
	case vk::Format::eR32Uint:
		return "eR32Uint"s;
		break;
	case vk::Format::eR32Sint:
		return "eR32Sint"s;
		break;
	case vk::Format::eR32Sfloat:
		return "eR32Sfloat"s;
		break;
	case vk::Format::eR32G32Uint:
		return "eR32G32Uint"s;
		break;
	case vk::Format::eR32G32Sint:
		return "eR32G32Sint"s;
		break;
	case vk::Format::eR32G32Sfloat:
		return "eR32G32Sfloat"s;
		break;
	case vk::Format::eR32G32B32Uint:
		return "eR32G32B32Uint"s;
		break;
	case vk::Format::eR32G32B32Sint:
		return "eR32G32B32Sint"s;
		break;
	case vk::Format::eR32G32B32Sfloat:
		return "eR32G32B32Sfloat"s;
		break;
	case vk::Format::eR32G32B32A32Uint:
		return "eR32G32B32A32Uint"s;
		break;
	case vk::Format::eR32G32B32A32Sint:
		return "eR32G32B32A32Sint"s;
		break;
	case vk::Format::eR32G32B32A32Sfloat:
		return "eR32G32B32A32Sfloat"s;
		break;
	case vk::Format::eR64Uint:
		return "eR64Uint"s;
		break;
	case vk::Format::eR64Sint:
		return "eR64Sint"s;
		break;
	case vk::Format::eR64Sfloat:
		return "eR64Sfloat"s;
		break;
	case vk::Format::eR64G64Uint:
		return "eR64G64Uint"s;
		break;
	case vk::Format::eR64G64Sint:
		return "eR64G64Sint"s;
		break;
	case vk::Format::eR64G64Sfloat:
		return "eR64G64Sfloat"s;
		break;
	case vk::Format::eR64G64B64Uint:
		return "eR64G64B64Uint"s;
		break;
	case vk::Format::eR64G64B64Sint:
		return "eR64G64B64Sint"s;
		break;
	case vk::Format::eR64G64B64Sfloat:
		return "eR64G64B64Sfloat"s;
		break;
	case vk::Format::eR64G64B64A64Uint:
		return "eR64G64B64A64Uint"s;
		break;
	case vk::Format::eR64G64B64A64Sint:
		return "eR64G64B64A64Sint"s;
		break;
	case vk::Format::eR64G64B64A64Sfloat:
		return "eR64G64B64A64Sfloat"s;
		break;
	case vk::Format::eB10G11R11UfloatPack32:
		return "eB10G11R11UfloatPack32"s;
		break;
	case vk::Format::eE5B9G9R9UfloatPack32:
		return "eE5B9G9R9UfloatPack32"s;
		break;
	case vk::Format::eD16Unorm:
		return "eD16Unorm"s;
		break;
	case vk::Format::eX8D24UnormPack32:
		return "eX8D24UnormPack32"s;
		break;
	case vk::Format::eD32Sfloat:
		return "eD32Sfloat"s;
		break;
	case vk::Format::eS8Uint:
		return "eS8Uint"s;
		break;
	case vk::Format::eD16UnormS8Uint:
		return "eD16UnormS8Uint"s;
		break;
	case vk::Format::eD24UnormS8Uint:
		return "eD24UnormS8Uint"s;
		break;
	case vk::Format::eD32SfloatS8Uint:
		return "eD32SfloatS8Uint"s;
		break;
	case vk::Format::eBc1RgbUnormBlock:
		break;
	case vk::Format::eBc1RgbSrgbBlock:
		break;
	case vk::Format::eBc1RgbaUnormBlock:
		break;
	case vk::Format::eBc1RgbaSrgbBlock:
		break;
	case vk::Format::eBc2UnormBlock:
		break;
	case vk::Format::eBc2SrgbBlock:
		break;
	case vk::Format::eBc3UnormBlock:
		break;
	case vk::Format::eBc3SrgbBlock:
		break;
	case vk::Format::eBc4UnormBlock:
		break;
	case vk::Format::eBc4SnormBlock:
		break;
	case vk::Format::eBc5UnormBlock:
		break;
	case vk::Format::eBc5SnormBlock:
		break;
	case vk::Format::eBc6HUfloatBlock:
		break;
	case vk::Format::eBc6HSfloatBlock:
		break;
	case vk::Format::eBc7UnormBlock:
		break;
	case vk::Format::eBc7SrgbBlock:
		break;
	case vk::Format::eEtc2R8G8B8UnormBlock:
		break;
	case vk::Format::eEtc2R8G8B8SrgbBlock:
		break;
	case vk::Format::eEtc2R8G8B8A1UnormBlock:
		break;
	case vk::Format::eEtc2R8G8B8A1SrgbBlock:
		break;
	case vk::Format::eEtc2R8G8B8A8UnormBlock:
		break;
	case vk::Format::eEtc2R8G8B8A8SrgbBlock:
		break;
	case vk::Format::eEacR11UnormBlock:
		break;
	case vk::Format::eEacR11SnormBlock:
		break;
	case vk::Format::eEacR11G11UnormBlock:
		break;
	case vk::Format::eEacR11G11SnormBlock:
		break;
	case vk::Format::eAstc4x4UnormBlock:
		break;
	case vk::Format::eAstc4x4SrgbBlock:
		break;
	case vk::Format::eAstc5x4UnormBlock:
		break;
	case vk::Format::eAstc5x4SrgbBlock:
		break;
	case vk::Format::eAstc5x5UnormBlock:
		break;
	case vk::Format::eAstc5x5SrgbBlock:
		break;
	case vk::Format::eAstc6x5UnormBlock:
		break;
	case vk::Format::eAstc6x5SrgbBlock:
		break;
	case vk::Format::eAstc6x6UnormBlock:
		break;
	case vk::Format::eAstc6x6SrgbBlock:
		break;
	case vk::Format::eAstc8x5UnormBlock:
		break;
	case vk::Format::eAstc8x5SrgbBlock:
		break;
	case vk::Format::eAstc8x6UnormBlock:
		break;
	case vk::Format::eAstc8x6SrgbBlock:
		break;
	case vk::Format::eAstc8x8UnormBlock:
		break;
	case vk::Format::eAstc8x8SrgbBlock:
		break;
	case vk::Format::eAstc10x5UnormBlock:
		break;
	case vk::Format::eAstc10x5SrgbBlock:
		break;
	case vk::Format::eAstc10x6UnormBlock:
		break;
	case vk::Format::eAstc10x6SrgbBlock:
		break;
	case vk::Format::eAstc10x8UnormBlock:
		break;
	case vk::Format::eAstc10x8SrgbBlock:
		break;
	case vk::Format::eAstc10x10UnormBlock:
		break;
	case vk::Format::eAstc10x10SrgbBlock:
		break;
	case vk::Format::eAstc12x10UnormBlock:
		break;
	case vk::Format::eAstc12x10SrgbBlock:
		break;
	case vk::Format::eAstc12x12UnormBlock:
		break;
	case vk::Format::eAstc12x12SrgbBlock:
		break;
	case vk::Format::eG8B8G8R8422Unorm:
		break;
	case vk::Format::eB8G8R8G8422Unorm:
		break;
	case vk::Format::eG8B8R83Plane420Unorm:
		break;
	case vk::Format::eG8B8R82Plane420Unorm:
		break;
	case vk::Format::eG8B8R83Plane422Unorm:
		break;
	case vk::Format::eG8B8R82Plane422Unorm:
		break;
	case vk::Format::eG8B8R83Plane444Unorm:
		break;
	case vk::Format::eR10X6UnormPack16:
		break;
	case vk::Format::eR10X6G10X6Unorm2Pack16:
		break;
	case vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16:
		break;
	case vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16:
		break;
	case vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16:
		break;
	case vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16:
		break;
	case vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16:
		break;
	case vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16:
		break;
	case vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16:
		break;
	case vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16:
		break;
	case vk::Format::eR12X4UnormPack16:
		break;
	case vk::Format::eR12X4G12X4Unorm2Pack16:
		break;
	case vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16:
		break;
	case vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16:
		break;
	case vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16:
		break;
	case vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16:
		break;
	case vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16:
		break;
	case vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16:
		break;
	case vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16:
		break;
	case vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16:
		break;
	case vk::Format::eG16B16G16R16422Unorm:
		break;
	case vk::Format::eB16G16R16G16422Unorm:
		break;
	case vk::Format::eG16B16R163Plane420Unorm:
		break;
	case vk::Format::eG16B16R162Plane420Unorm:
		break;
	case vk::Format::eG16B16R163Plane422Unorm:
		break;
	case vk::Format::eG16B16R162Plane422Unorm:
		break;
	case vk::Format::eG16B16R163Plane444Unorm:
		break;
	case vk::Format::eG8B8R82Plane444Unorm:
		break;
	case vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16:
		break;
	case vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16:
		break;
	case vk::Format::eG16B16R162Plane444Unorm:
		break;
	case vk::Format::eA4R4G4B4UnormPack16:
		break;
	case vk::Format::eA4B4G4R4UnormPack16:
		break;
	case vk::Format::eAstc4x4SfloatBlock:
		break;
	case vk::Format::eAstc5x4SfloatBlock:
		break;
	case vk::Format::eAstc5x5SfloatBlock:
		break;
	case vk::Format::eAstc6x5SfloatBlock:
		break;
	case vk::Format::eAstc6x6SfloatBlock:
		break;
	case vk::Format::eAstc8x5SfloatBlock:
		break;
	case vk::Format::eAstc8x6SfloatBlock:
		break;
	case vk::Format::eAstc8x8SfloatBlock:
		break;
	case vk::Format::eAstc10x5SfloatBlock:
		break;
	case vk::Format::eAstc10x6SfloatBlock:
		break;
	case vk::Format::eAstc10x8SfloatBlock:
		break;
	case vk::Format::eAstc10x10SfloatBlock:
		break;
	case vk::Format::eAstc12x10SfloatBlock:
		break;
	case vk::Format::eAstc12x12SfloatBlock:
		break;
	case vk::Format::ePvrtc12BppUnormBlockIMG:
		break;
	case vk::Format::ePvrtc14BppUnormBlockIMG:
		break;
	case vk::Format::ePvrtc22BppUnormBlockIMG:
		break;
	case vk::Format::ePvrtc24BppUnormBlockIMG:
		break;
	case vk::Format::ePvrtc12BppSrgbBlockIMG:
		break;
	case vk::Format::ePvrtc14BppSrgbBlockIMG:
		break;
	case vk::Format::ePvrtc22BppSrgbBlockIMG:
		break;
	case vk::Format::ePvrtc24BppSrgbBlockIMG:
		break;
	default:
		return "Unknown"s;
		break;
	}
	return "Unspecified"s;
}
inline std::string toString(vk::ColorSpaceKHR val)
{
	switch (val)
	{
	case vk::ColorSpaceKHR::eSrgbNonlinear:
		return "SRGB nonlinear"s;
		break;
	case vk::ColorSpaceKHR::eDisplayP3NonlinearEXT:
		return "Display P3 nonlinear"s;
		break;
	case vk::ColorSpaceKHR::eExtendedSrgbLinearEXT:
		return "Extended SRGB linear"s;
		break;
	case vk::ColorSpaceKHR::eDisplayP3LinearEXT:
		return "Display P3 linear"s;
		break;
	case vk::ColorSpaceKHR::eDciP3NonlinearEXT:
		return "DCI P3 nonlinear"s;
		break;
	case vk::ColorSpaceKHR::eBt709LinearEXT:
		return "BT709 linear"s;
		break;
	case vk::ColorSpaceKHR::eBt709NonlinearEXT:
		return "BT709 nonlinear"s;
		break;
	case vk::ColorSpaceKHR::eBt2020LinearEXT:
		return "BT2020 linear"s;
		break;
	case vk::ColorSpaceKHR::eHdr10St2084EXT:
		return "HDR 10 ST 2084"s;
		break;
	case vk::ColorSpaceKHR::eDolbyvisionEXT:
		return "Dolbyvision"s;
		break;
	case vk::ColorSpaceKHR::eHdr10HlgEXT:
		return "HDR 10 HLG"s;
		break;
	case vk::ColorSpaceKHR::eAdobergbLinearEXT:
		return "Adobe RGB linear"s;
		break;
	case vk::ColorSpaceKHR::eAdobergbNonlinearEXT:
		return "Adobe RGB nonlinear"s;
		break;
	case vk::ColorSpaceKHR::ePassThroughEXT:
		return "Pass Through"s;
		break;
	case vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT:
		return "Extended SRGB nonlinear"s;
		break;
	case vk::ColorSpaceKHR::eDisplayNativeAMD:
		return "Display native"s;
		break;
	default:
		return "Undefined"s;
		break;
	}
}
inline std::string toString(vk::PresentModeKHR val)
{
	switch (val)
	{
	case vk::PresentModeKHR::eImmediate:
		return "Immediate"s;
		break;
	case vk::PresentModeKHR::eMailbox:
		return "Mailbox"s;
		break;
	case vk::PresentModeKHR::eFifo:
		return "FIFO"s;
		break;
	case vk::PresentModeKHR::eFifoRelaxed:
		return "FIFO relaxed"s;
		break;
	case vk::PresentModeKHR::eSharedDemandRefresh:
		return "Shared demand refresh"s;
		break;
	case vk::PresentModeKHR::eSharedContinuousRefresh:
		return "Shared continuous refresh"s;
		break;
	default:
		return "Undefined"s;
		break;
	}
}
inline std::string toString(vk::Result val)
{
	switch (val)
	{
	case vk::Result::eSuccess:
		return "Success"s;
		break;
	case vk::Result::eNotReady:
		return "Not ready"s;
		break;
	case vk::Result::eTimeout:
		return "Timeout"s;
		break;
	case vk::Result::eEventSet:
		return "Event set"s;
		break;
	case vk::Result::eEventReset:
		return "Event reset"s;
		break;
	case vk::Result::eIncomplete:
		return "Incomplete"s;
		break;
	case vk::Result::eErrorOutOfHostMemory:
		return "Error out of host memory"s;
		break;
	case vk::Result::eErrorOutOfDeviceMemory:
		return "Error out of device memory"s;
		break;
	case vk::Result::eErrorInitializationFailed:
		return "Error initialization failed"s;
		break;
	case vk::Result::eErrorDeviceLost:
		return "Error device lost"s;
		break;
	case vk::Result::eErrorMemoryMapFailed:
		return "Error memory map failed"s;
		break;
	case vk::Result::eErrorLayerNotPresent:
		return "Error layer not present"s;
		break;
	case vk::Result::eErrorExtensionNotPresent:
		return "Error extension not present"s;
		break;
	case vk::Result::eErrorFeatureNotPresent:
		return "Error feature not present"s;
		break;
	case vk::Result::eErrorIncompatibleDriver:
		return "Error incompatible driver"s;
		break;
	case vk::Result::eErrorTooManyObjects:
		return "Error too many objects"s;
		break;
	case vk::Result::eErrorFormatNotSupported:
		return "Error format not supported"s;
		break;
	case vk::Result::eErrorFragmentedPool:
		return "Error fragmented pool"s;
		break;
	case vk::Result::eErrorUnknown:
		return "Error unknown"s;
		break;
	case vk::Result::eErrorOutOfPoolMemory:
		return "Error out of pool memory"s;
		break;
	case vk::Result::eErrorInvalidExternalHandle:
		return "Error invalid external handle"s;
		break;
	case vk::Result::eErrorFragmentation:
		return "Error fragmentation"s;
		break;
	case vk::Result::eErrorInvalidOpaqueCaptureAddress:
		return "Error invalid opaque capture address"s;
		break;
	case vk::Result::ePipelineCompileRequired:
		return "Pipeline compile required"s;
		break;
	case vk::Result::eErrorSurfaceLostKHR:
		return "Error surface lost"s;
		break;
	case vk::Result::eErrorNativeWindowInUseKHR:
		return "Error native window in use"s;
		break;
	case vk::Result::eSuboptimalKHR:
		return "Suboptimal"s;
		break;
	case vk::Result::eErrorOutOfDateKHR:
		return "Error out of date"s;
		break;
	case vk::Result::eErrorIncompatibleDisplayKHR:
		return "Error incompatible display"s;
		break;
	case vk::Result::eErrorValidationFailedEXT:
		return "Error validation failed"s;
		break;
	case vk::Result::eErrorInvalidShaderNV:
		return "Error invalid shader"s;
		break;
	case vk::Result::eErrorInvalidDrmFormatModifierPlaneLayoutEXT:
		return "Error invalid drm format modifier plane layout"s;
		break;
	case vk::Result::eErrorNotPermittedKHR:
		return "Error not permitted"s;
		break;
	case vk::Result::eThreadIdleKHR:
		return "Thread idle"s;
		break;
	case vk::Result::eThreadDoneKHR:
		return "Thread done"s;
		break;
	case vk::Result::eOperationDeferredKHR:
		return "Operation deferred"s;
		break;
	case vk::Result::eOperationNotDeferredKHR:
		return "Operation not deferred"s;
		break;
	case vk::Result::eErrorCompressionExhaustedEXT:
		return "Error compression exhausted"s;
		break;
	default:
		return "Undefined"s;
		break;
	}
}
inline std::string toString(vk::MemoryPropertyFlagBits val)
{
	switch (val)
	{
	case vk::MemoryPropertyFlagBits::eDeviceLocal:
		return "Device local"s;
		break;
	case vk::MemoryPropertyFlagBits::eHostVisible:
		return "Host visible"s;
		break;
	case vk::MemoryPropertyFlagBits::eHostCoherent:
		return "Host coherent"s;
		break;
	case vk::MemoryPropertyFlagBits::eHostCached:
		return "Host cached"s;
		break;
	case vk::MemoryPropertyFlagBits::eLazilyAllocated:
		return "Lazily allocated"s;
		break;
	case vk::MemoryPropertyFlagBits::eProtected:
		return "Protected"s;
		break;
	case vk::MemoryPropertyFlagBits::eDeviceCoherentAMD:
		return "Device coherent"s;
		break;
	case vk::MemoryPropertyFlagBits::eDeviceUncachedAMD:
		return "Device uncached"s;
		break;
	case vk::MemoryPropertyFlagBits::eRdmaCapableNV:
		return "RDMA capable"s;
		break;
	default:
		return "Undefined"s;
		break;
	}
}
inline std::string toString(vk::MemoryPropertyFlags val)
{
	auto result = ""s;
	result += val & vk::MemoryPropertyFlagBits::eDeviceLocal ? toString(vk::MemoryPropertyFlagBits::eDeviceLocal) + " "s : ""s;
	result += val & vk::MemoryPropertyFlagBits::eHostVisible ? toString(vk::MemoryPropertyFlagBits::eHostVisible) + " "s : ""s;
	result += val & vk::MemoryPropertyFlagBits::eHostCoherent ? toString(vk::MemoryPropertyFlagBits::eHostCoherent) + " "s : ""s;
	result += val & vk::MemoryPropertyFlagBits::eHostCached ? toString(vk::MemoryPropertyFlagBits::eHostCached) + " "s : ""s;
	result += val & vk::MemoryPropertyFlagBits::eLazilyAllocated ? toString(vk::MemoryPropertyFlagBits::eLazilyAllocated) + " "s : ""s;
	result += val & vk::MemoryPropertyFlagBits::eProtected ? toString(vk::MemoryPropertyFlagBits::eProtected) + " "s : ""s;
	result += val & vk::MemoryPropertyFlagBits::eDeviceCoherentAMD ? toString(vk::MemoryPropertyFlagBits::eDeviceCoherentAMD) + " "s : ""s;
	result += val & vk::MemoryPropertyFlagBits::eDeviceUncachedAMD ? toString(vk::MemoryPropertyFlagBits::eDeviceUncachedAMD) + " "s : ""s;
	result += val & vk::MemoryPropertyFlagBits::eRdmaCapableNV ? toString(vk::MemoryPropertyFlagBits::eRdmaCapableNV) + " "s : ""s;
	return result;
}
inline std::string toString(vk::MemoryType type)
{
	return std::vformat("{{heap index: {}, properties: {}}}", std::make_format_args(type.heapIndex, toString(type.propertyFlags)));
}
inline std::string toString(vk::MemoryHeapFlagBits val)
{
	switch (val)
	{
	case vk::MemoryHeapFlagBits::eDeviceLocal:
		return "Device local"s;
		break;
	case vk::MemoryHeapFlagBits::eMultiInstance:
		return "Multi instance"s;
		break;
	default:
		return "Undefined"s;
		break;
	}
}
inline std::string toString(vk::MemoryHeapFlags val)
{
	auto result = ""s;
	result += val & vk::MemoryHeapFlagBits::eDeviceLocal ? toString(vk::MemoryHeapFlagBits::eDeviceLocal) + " "s : ""s;
	result += val & vk::MemoryHeapFlagBits::eMultiInstance ? toString(vk::MemoryHeapFlagBits::eMultiInstance) + " "s : ""s;
	return result;
}
inline std::string toString(vk::MemoryHeap heap)
{
	return std::vformat("{{size: {}, flags: {}}}", std::make_format_args(heap.size, toString(heap.flags)));
}
template<class T>
inline std::string toString() = delete;
template<>
inline std::string toString<vk::Instance>()
{
	return "Instance"s;
}
template<>
inline std::string toString<vk::PhysicalDevice>()
{
	return "Physical device"s;
}
template<>
inline std::string toString<vk::LayerProperties>()
{
	return "Validation layer"s;
}
template<>
inline std::string toString<vk::SurfaceCapabilitiesKHR>()
{
	return "Surface capabilities"s;
}
template<>
inline std::string toString<vk::SurfaceFormatKHR>()
{
	return "Surface format"s;
}
template<>
inline std::string toString<vk::PresentModeKHR>()
{
	return "Present mode"s;
}
template<>
inline std::string toString<vk::QueueFamilyProperties>()
{
	return "Queue family"s;
}
template<>
inline std::string toString<vk::Image>()
{
	return "Image"s;
}
template<class T, class V>
inline std::string toString() {}
template<>
inline std::string toString<vk::Instance, vk::ExtensionProperties>()
{
	return "Instance extension"s;
}
template<>
inline std::string toString<vk::PhysicalDevice, vk::ExtensionProperties>()
{
	return "Physical device extension"s;
}

//prints out according to format
template<class... Args>
inline void formatPrint(std::ostream& ostr, std::string_view format, Args&&... args)
{
	ostr << std::vformat(format, std::make_format_args(std::forward<Args>(args)...));
}

//helper that ties struct values to labels
template<class T>
struct LabelValuePair
{
	LabelValuePair(std::string const& label, T const& value)
		:label(label), value(value)
	{}

	std::string label;
	T value;
};

//recursively constructs string with all value pairs
inline std::string getLabelValuePairsString() { return ""s; }
template<class T, class... Pairs>
inline std::string getLabelValuePairsString(LabelValuePair<T> const& pair, Pairs&&... pairs)
{
	return std::vformat("\t"s + pair.label + ": {}"s + (sizeof...(pairs) != 0 ? ","s : ""s), std::make_format_args(toString(pair.value))) +
		getLabelValuePairsString(std::forward<Pairs>(pairs)...);
}

//get string for properly printing class
inline auto getFormatString(vk::ExtensionProperties const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Extension name"s, val.extensionName},
									LabelValuePair{"Version"s, val.specVersion});
}
inline auto getFormatString(vk::LayerProperties const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Extension name"s, val.layerName},
									LabelValuePair{"Version"s, val.implementationVersion},
									LabelValuePair{"Description"s, val.description});
}
inline auto getFormatString(vk::PhysicalDeviceLimits const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Max image dimension 1D"s, val.maxImageDimension1D},
									LabelValuePair{"maxImageDimension2D"s, val.maxImageDimension2D},
									LabelValuePair{"maxImageDimension3D"s, val.maxImageDimension3D},
									LabelValuePair{"maxImageDimensionCube"s, val.maxImageDimensionCube},
									LabelValuePair{"maxImageArrayLayers"s, val.maxImageArrayLayers},
									LabelValuePair{"maxTexelBufferElements"s, val.maxTexelBufferElements},
									LabelValuePair{"maxUniformBufferRange"s, val.maxUniformBufferRange},
									LabelValuePair{"maxStorageBufferRange"s, val.maxStorageBufferRange},
									LabelValuePair{"maxPushConstantsSize"s, val.maxPushConstantsSize},
									LabelValuePair{"maxMemoryAllocationCount"s, val.maxMemoryAllocationCount},
									LabelValuePair{"maxSamplerAllocationCount"s, val.maxSamplerAllocationCount},
									LabelValuePair{"bufferImageGranularity"s, val.bufferImageGranularity},
									LabelValuePair{"sparseAddressSpaceSize"s, val.sparseAddressSpaceSize},
									LabelValuePair{"maxBoundDescriptorSets"s, val.maxBoundDescriptorSets},
									LabelValuePair{"maxPerStageDescriptorSamplers"s, val.maxPerStageDescriptorSamplers},
									LabelValuePair{"maxPerStageDescriptorUniformBuffers"s, val.maxPerStageDescriptorUniformBuffers},
									LabelValuePair{"maxPerStageDescriptorStorageBuffers"s, val.maxPerStageDescriptorStorageBuffers},
									LabelValuePair{"maxPerStageDescriptorSampledImages"s, val.maxPerStageDescriptorSampledImages},
									LabelValuePair{"maxPerStageDescriptorStorageImages"s, val.maxPerStageDescriptorStorageImages},
									LabelValuePair{"maxPerStageDescriptorInputAttachments"s, val.maxPerStageDescriptorInputAttachments},
									LabelValuePair{"maxPerStageResources"s, val.maxPerStageResources},
									LabelValuePair{"maxDescriptorSetSamplers"s, val.maxDescriptorSetSamplers},
									LabelValuePair{"maxDescriptorSetUniformBuffers"s, val.maxDescriptorSetUniformBuffers},
									LabelValuePair{"maxDescriptorSetUniformBuffersDynamic"s, val.maxDescriptorSetUniformBuffersDynamic},
									LabelValuePair{"maxDescriptorSetStorageBuffers"s, val.maxDescriptorSetStorageBuffers},
									LabelValuePair{"maxDescriptorSetStorageBuffersDynamic"s, val.maxDescriptorSetStorageBuffersDynamic},
									LabelValuePair{"maxDescriptorSetSampledImages"s, val.maxDescriptorSetSampledImages},
									LabelValuePair{"maxDescriptorSetStorageImages"s, val.maxDescriptorSetStorageImages},
									LabelValuePair{"maxDescriptorSetInputAttachments"s, val.maxDescriptorSetInputAttachments},
									LabelValuePair{"maxVertexInputAttributes"s, val.maxVertexInputAttributes},
									LabelValuePair{"maxVertexInputBindings"s, val.maxVertexInputBindings},
									LabelValuePair{"maxVertexInputAttributeOffset"s, val.maxVertexInputAttributeOffset},
									LabelValuePair{"maxVertexInputBindingStride"s, val.maxVertexInputBindingStride},
									LabelValuePair{"maxVertexOutputComponents"s, val.maxVertexOutputComponents},
									LabelValuePair{"maxTessellationGenerationLevel"s, val.maxTessellationGenerationLevel},
									LabelValuePair{"maxTessellationPatchSize"s, val.maxTessellationPatchSize},
									LabelValuePair{"maxTessellationControlPerVertexInputComponents"s, val.maxTessellationControlPerVertexInputComponents},
									LabelValuePair{"maxTessellationControlPerVertexOutputComponents"s, val.maxTessellationControlPerVertexOutputComponents},
									LabelValuePair{"maxTessellationControlPerPatchOutputComponents"s, val.maxTessellationControlPerPatchOutputComponents},
									LabelValuePair{"maxTessellationControlTotalOutputComponents"s, val.maxTessellationControlTotalOutputComponents},
									LabelValuePair{"maxTessellationEvaluationInputComponents"s, val.maxTessellationEvaluationInputComponents},
									LabelValuePair{"maxTessellationEvaluationOutputComponents"s, val.maxTessellationEvaluationOutputComponents},
									LabelValuePair{"maxGeometryShaderInvocations"s, val.maxGeometryShaderInvocations},
									LabelValuePair{"maxGeometryInputComponents"s, val.maxGeometryInputComponents},
									LabelValuePair{"maxGeometryOutputComponents"s, val.maxGeometryOutputComponents},
									LabelValuePair{"maxGeometryOutputVertices"s, val.maxGeometryOutputVertices},
									LabelValuePair{"maxGeometryTotalOutputComponents"s, val.maxGeometryTotalOutputComponents},
									LabelValuePair{"maxFragmentInputComponents"s, val.maxFragmentInputComponents},
									LabelValuePair{"maxFragmentOutputAttachments"s, val.maxFragmentOutputAttachments},
									LabelValuePair{"maxFragmentDualSrcAttachments"s, val.maxFragmentDualSrcAttachments},
									LabelValuePair{"maxFragmentCombinedOutputResources"s, val.maxFragmentCombinedOutputResources},
									LabelValuePair{"maxComputeSharedMemorySize"s, val.maxComputeSharedMemorySize},
									LabelValuePair{"maxComputeWorkGroupCount"s, val.maxComputeWorkGroupCount},
									LabelValuePair{"maxComputeWorkGroupInvocations"s, val.maxComputeWorkGroupInvocations},
									LabelValuePair{"maxComputeWorkGroupSize"s, val.maxComputeWorkGroupSize},
									LabelValuePair{"subPixelPrecisionBits"s, val.subPixelPrecisionBits},
									LabelValuePair{"subTexelPrecisionBits"s, val.subTexelPrecisionBits},
									LabelValuePair{"mipmapPrecisionBits"s, val.mipmapPrecisionBits},
									LabelValuePair{"maxDrawIndexedIndexValue"s, val.maxDrawIndexedIndexValue},
									LabelValuePair{"maxDrawIndirectCount"s, val.maxDrawIndirectCount},
									LabelValuePair{"maxSamplerLodBias"s, val.maxSamplerLodBias},
									LabelValuePair{"maxSamplerAnisotropy"s, val.maxSamplerAnisotropy},
									LabelValuePair{"maxViewports"s, val.maxViewports},
									LabelValuePair{"maxViewportDimensions"s, val.maxViewportDimensions},
									LabelValuePair{"viewportBoundsRange"s, val.viewportBoundsRange},
									LabelValuePair{"viewportSubPixelBits"s, val.viewportSubPixelBits},
									LabelValuePair{"minMemoryMapAlignment"s, val.minMemoryMapAlignment},
									LabelValuePair{"minTexelBufferOffsetAlignment"s, val.minTexelBufferOffsetAlignment},
									LabelValuePair{"minUniformBufferOffsetAlignment"s, val.minUniformBufferOffsetAlignment},
									LabelValuePair{"minStorageBufferOffsetAlignment"s, val.minStorageBufferOffsetAlignment},
									LabelValuePair{"minTexelOffset"s, val.minTexelOffset},
									LabelValuePair{"maxTexelOffset"s, val.maxTexelOffset},
									LabelValuePair{"minTexelGatherOffset"s, val.minTexelGatherOffset},
									LabelValuePair{"maxTexelGatherOffset"s, val.maxTexelGatherOffset},
									LabelValuePair{"minInterpolationOffset"s, val.minInterpolationOffset},
									LabelValuePair{"maxInterpolationOffset"s, val.maxInterpolationOffset},
									LabelValuePair{"subPixelInterpolationOffsetBits"s, val.subPixelInterpolationOffsetBits},
									LabelValuePair{"maxFramebufferWidth"s, val.maxFramebufferWidth},
									LabelValuePair{"maxFramebufferHeight"s, val.maxFramebufferHeight},
									LabelValuePair{"maxFramebufferLayers"s, val.maxFramebufferLayers},
									LabelValuePair{"framebufferColorSampleCounts"s, val.framebufferColorSampleCounts},
									LabelValuePair{"framebufferDepthSampleCounts"s, val.framebufferDepthSampleCounts},
									LabelValuePair{"framebufferStencilSampleCounts"s, val.framebufferStencilSampleCounts},
									LabelValuePair{"framebufferNoAttachmentsSampleCounts"s, val.framebufferNoAttachmentsSampleCounts},
									LabelValuePair{"maxColorAttachments"s, val.maxColorAttachments},
									LabelValuePair{"sampledImageColorSampleCounts"s, val.sampledImageColorSampleCounts},
									LabelValuePair{"sampledImageIntegerSampleCounts"s, val.sampledImageIntegerSampleCounts},
									LabelValuePair{"sampledImageDepthSampleCounts"s, val.sampledImageDepthSampleCounts},
									LabelValuePair{"sampledImageStencilSampleCounts"s, val.sampledImageStencilSampleCounts},
									LabelValuePair{"storageImageSampleCounts"s, val.storageImageSampleCounts},
									LabelValuePair{"maxSampleMaskWords"s, val.maxSampleMaskWords},
									LabelValuePair{"timestampComputeAndGraphics"s, val.timestampComputeAndGraphics},
									LabelValuePair{"timestampPeriod"s, val.timestampPeriod},
									LabelValuePair{"maxClipDistances"s, val.maxClipDistances},
									LabelValuePair{"maxCullDistances"s, val.maxCullDistances},
									LabelValuePair{"maxCombinedClipAndCullDistances"s, val.maxCombinedClipAndCullDistances},
									LabelValuePair{"discreteQueuePriorities"s, val.discreteQueuePriorities},
									LabelValuePair{"pointSizeRange"s, val.pointSizeRange},
									LabelValuePair{"lineWidthRange"s, val.lineWidthRange},
									LabelValuePair{"pointSizeGranularity"s, val.pointSizeGranularity},
									LabelValuePair{"lineWidthGranularity"s, val.lineWidthGranularity},
									LabelValuePair{"strictLines"s, val.strictLines},
									LabelValuePair{"standardSampleLocations"s, val.standardSampleLocations},
									LabelValuePair{"optimalBufferCopyOffsetAlignment"s, val.optimalBufferCopyOffsetAlignment},
									LabelValuePair{"optimalBufferCopyRowPitchAlignment"s, val.optimalBufferCopyRowPitchAlignment},
									LabelValuePair{"nonCoherentAtomSize"s, val.nonCoherentAtomSize},
									LabelValuePair{"maxImageDimension2D"s, val.maxImageDimension2D},
									LabelValuePair{"maxImageDimension3D"s, val.maxImageDimension3D},
									LabelValuePair{"maxImageDimensionCube"s, val.maxImageDimensionCube},
									LabelValuePair{"maxImageArrayLayers"s, val.maxImageArrayLayers},
									LabelValuePair{"maxTexelBufferElements"s, val.maxTexelBufferElements},
									LabelValuePair{"maxUniformBufferRange"s, val.maxUniformBufferRange},
									LabelValuePair{"maxStorageBufferRange"s, val.maxStorageBufferRange},
									LabelValuePair{"maxPushConstantsSize"s, val.maxPushConstantsSize},
									LabelValuePair{"maxMemoryAllocationCount"s, val.maxMemoryAllocationCount},
									LabelValuePair{"maxSamplerAllocationCount"s, val.maxSamplerAllocationCount},
									LabelValuePair{"bufferImageGranularity"s, val.bufferImageGranularity},
									LabelValuePair{"sparseAddressSpaceSize"s, val.sparseAddressSpaceSize},
									LabelValuePair{"maxBoundDescriptorSets"s, val.maxBoundDescriptorSets},
									LabelValuePair{"maxPerStageDescriptorSamplers"s, val.maxPerStageDescriptorSamplers},
									LabelValuePair{"maxPerStageDescriptorUniformBuffers"s, val.maxPerStageDescriptorUniformBuffers},
									LabelValuePair{"maxPerStageDescriptorStorageBuffers"s, val.maxPerStageDescriptorStorageBuffers},
									LabelValuePair{"maxPerStageDescriptorSampledImages"s, val.maxPerStageDescriptorSampledImages},
									LabelValuePair{"maxPerStageDescriptorStorageImages"s, val.maxPerStageDescriptorStorageImages},
									LabelValuePair{"maxPerStageDescriptorInputAttachments"s, val.maxPerStageDescriptorInputAttachments},
									LabelValuePair{"maxPerStageResources"s, val.maxPerStageResources},
									LabelValuePair{"maxDescriptorSetSamplers"s, val.maxDescriptorSetSamplers},
									LabelValuePair{"maxDescriptorSetUniformBuffers"s, val.maxDescriptorSetUniformBuffers},
									LabelValuePair{"maxDescriptorSetUniformBuffersDynamic"s, val.maxDescriptorSetUniformBuffersDynamic},
									LabelValuePair{"maxDescriptorSetStorageBuffers"s, val.maxDescriptorSetStorageBuffers},
									LabelValuePair{"maxDescriptorSetStorageBuffersDynamic"s, val.maxDescriptorSetStorageBuffersDynamic},
									LabelValuePair{"maxDescriptorSetSampledImages"s, val.maxDescriptorSetSampledImages},
									LabelValuePair{"maxDescriptorSetStorageImages"s, val.maxDescriptorSetStorageImages},
									LabelValuePair{"maxDescriptorSetInputAttachments"s, val.maxDescriptorSetInputAttachments},
									LabelValuePair{"maxVertexInputAttributes"s, val.maxVertexInputAttributes},
									LabelValuePair{"maxVertexInputBindings"s, val.maxVertexInputBindings},
									LabelValuePair{"maxVertexInputAttributeOffset"s, val.maxVertexInputAttributeOffset},
									LabelValuePair{"maxVertexInputBindingStride"s, val.maxVertexInputBindingStride},
									LabelValuePair{"maxVertexOutputComponents"s, val.maxVertexOutputComponents},
									LabelValuePair{"maxTessellationGenerationLevel"s, val.maxTessellationGenerationLevel},
									LabelValuePair{"maxTessellationPatchSize"s, val.maxTessellationPatchSize},
									LabelValuePair{"maxTessellationControlPerVertexInputComponents"s, val.maxTessellationControlPerVertexInputComponents},
									LabelValuePair{"maxTessellationControlPerVertexOutputComponents"s, val.maxTessellationControlPerVertexOutputComponents},
									LabelValuePair{"maxTessellationControlPerPatchOutputComponents"s, val.maxTessellationControlPerPatchOutputComponents},
									LabelValuePair{"maxTessellationControlTotalOutputComponents"s, val.maxTessellationControlTotalOutputComponents},
									LabelValuePair{"maxTessellationEvaluationInputComponents"s, val.maxTessellationEvaluationInputComponents},
									LabelValuePair{"maxTessellationEvaluationOutputComponents"s, val.maxTessellationEvaluationOutputComponents},
									LabelValuePair{"maxGeometryShaderInvocations"s, val.maxGeometryShaderInvocations},
									LabelValuePair{"maxGeometryInputComponents"s, val.maxGeometryInputComponents},
									LabelValuePair{"maxGeometryOutputComponents"s, val.maxGeometryOutputComponents},
									LabelValuePair{"maxGeometryOutputVertices"s, val.maxGeometryOutputVertices},
									LabelValuePair{"maxGeometryTotalOutputComponents"s, val.maxGeometryTotalOutputComponents},
									LabelValuePair{"maxFragmentInputComponents"s, val.maxFragmentInputComponents},
									LabelValuePair{"maxFragmentOutputAttachments"s, val.maxFragmentOutputAttachments},
									LabelValuePair{"maxFragmentDualSrcAttachments"s, val.maxFragmentDualSrcAttachments},
									LabelValuePair{"maxFragmentCombinedOutputResources"s, val.maxFragmentCombinedOutputResources},
									LabelValuePair{"maxComputeSharedMemorySize"s, val.maxComputeSharedMemorySize},
									LabelValuePair{"maxComputeWorkGroupCount"s, val.maxComputeWorkGroupCount},
									LabelValuePair{"maxComputeWorkGroupInvocations"s, val.maxComputeWorkGroupInvocations},
									LabelValuePair{"maxComputeWorkGroupSize"s, val.maxComputeWorkGroupSize},
									LabelValuePair{"subPixelPrecisionBits"s, val.subPixelPrecisionBits},
									LabelValuePair{"subTexelPrecisionBits"s, val.subTexelPrecisionBits},
									LabelValuePair{"mipmapPrecisionBits"s, val.mipmapPrecisionBits},
									LabelValuePair{"maxDrawIndexedIndexValue"s, val.maxDrawIndexedIndexValue},
									LabelValuePair{"maxDrawIndirectCount"s, val.maxDrawIndirectCount},
									LabelValuePair{"maxSamplerLodBias"s, val.maxSamplerLodBias},
									LabelValuePair{"maxSamplerAnisotropy"s, val.maxSamplerAnisotropy},
									LabelValuePair{"maxViewports"s, val.maxViewports},
									LabelValuePair{"maxViewportDimensions"s, val.maxViewportDimensions},
									LabelValuePair{"viewportBoundsRange"s, val.viewportBoundsRange},
									LabelValuePair{"viewportSubPixelBits"s, val.viewportSubPixelBits},
									LabelValuePair{"minMemoryMapAlignment"s, val.minMemoryMapAlignment},
									LabelValuePair{"minTexelBufferOffsetAlignment"s, val.minTexelBufferOffsetAlignment},
									LabelValuePair{"minUniformBufferOffsetAlignment"s, val.minUniformBufferOffsetAlignment},
									LabelValuePair{"minStorageBufferOffsetAlignment"s, val.minStorageBufferOffsetAlignment},
									LabelValuePair{"minTexelOffset"s, val.minTexelOffset},
									LabelValuePair{"maxTexelOffset"s, val.maxTexelOffset},
									LabelValuePair{"minTexelGatherOffset"s, val.minTexelGatherOffset},
									LabelValuePair{"maxTexelGatherOffset"s, val.maxTexelGatherOffset},
									LabelValuePair{"minInterpolationOffset"s, val.minInterpolationOffset},
									LabelValuePair{"maxInterpolationOffset"s, val.maxInterpolationOffset},
									LabelValuePair{"subPixelInterpolationOffsetBits"s, val.subPixelInterpolationOffsetBits},
									LabelValuePair{"maxFramebufferWidth"s, val.maxFramebufferWidth},
									LabelValuePair{"maxFramebufferHeight"s, val.maxFramebufferHeight},
									LabelValuePair{"maxFramebufferLayers"s, val.maxFramebufferLayers},
									LabelValuePair{"framebufferColorSampleCounts"s, val.framebufferColorSampleCounts},
									LabelValuePair{"framebufferDepthSampleCounts"s, val.framebufferDepthSampleCounts},
									LabelValuePair{"framebufferStencilSampleCounts"s, val.framebufferStencilSampleCounts},
									LabelValuePair{"framebufferNoAttachmentsSampleCounts"s, val.framebufferNoAttachmentsSampleCounts},
									LabelValuePair{"maxColorAttachments"s, val.maxColorAttachments},
									LabelValuePair{"sampledImageColorSampleCounts"s, val.sampledImageColorSampleCounts},
									LabelValuePair{"sampledImageIntegerSampleCounts"s, val.sampledImageIntegerSampleCounts},
									LabelValuePair{"sampledImageDepthSampleCounts"s, val.sampledImageDepthSampleCounts},
									LabelValuePair{"sampledImageStencilSampleCounts"s, val.sampledImageStencilSampleCounts},
									LabelValuePair{"storageImageSampleCounts"s, val.storageImageSampleCounts},
									LabelValuePair{"maxSampleMaskWords"s, val.maxSampleMaskWords},
									LabelValuePair{"timestampComputeAndGraphics"s, val.timestampComputeAndGraphics},
									LabelValuePair{"timestampPeriod"s, val.timestampPeriod},
									LabelValuePair{"maxClipDistances"s, val.maxClipDistances},
									LabelValuePair{"maxCullDistances"s, val.maxCullDistances},
									LabelValuePair{"maxCombinedClipAndCullDistances"s, val.maxCombinedClipAndCullDistances},
									LabelValuePair{"discreteQueuePriorities"s, val.discreteQueuePriorities},
									LabelValuePair{"pointSizeRange"s, val.pointSizeRange},
									LabelValuePair{"lineWidthRange"s, val.lineWidthRange},
									LabelValuePair{"pointSizeGranularity"s, val.pointSizeGranularity},
									LabelValuePair{"lineWidthGranularity"s, val.lineWidthGranularity},
									LabelValuePair{"strictLines"s, val.strictLines},
									LabelValuePair{"standardSampleLocations"s, val.standardSampleLocations},
									LabelValuePair{"optimalBufferCopyOffsetAlignment"s, val.optimalBufferCopyOffsetAlignment},
									LabelValuePair{"optimalBufferCopyRowPitchAlignment"s, val.optimalBufferCopyRowPitchAlignment},
									LabelValuePair{"nonCoherentAtomSize"s, val.nonCoherentAtomSize}
	);
}
inline auto getFormatString(vk::PhysicalDeviceProperties const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Device name"s, val.deviceName},
									LabelValuePair{"Device type"s, val.deviceType},
									LabelValuePair{"API version"s, val.apiVersion},
									LabelValuePair{"Driver version"s, val.driverVersion},
									LabelValuePair{"Vendor ID"s, val.vendorID},
									LabelValuePair{"Device ID"s, val.deviceID}) + ","s + getFormatString(val.limits);
}
inline auto getFormatString(vk::PhysicalDeviceFeatures const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Robust buffer access"s, bool(val.robustBufferAccess)},
									LabelValuePair{"Full draw index Uint32"s, bool(val.fullDrawIndexUint32)},
									LabelValuePair{"Image cube array"s, bool(val.imageCubeArray)},
									LabelValuePair{"Independent blend"s, bool(val.independentBlend)},
									LabelValuePair{"Geometry shader"s, bool(val.geometryShader)},
									LabelValuePair{"Tessellation shader"s, bool(val.tessellationShader)},
									LabelValuePair{"Sample rate shading"s, bool(val.sampleRateShading)},
									LabelValuePair{"Dual source blend"s, bool(val.dualSrcBlend)},
									LabelValuePair{"Logic operation"s, bool(val.logicOp)},
									LabelValuePair{"Multi draw indirect"s, bool(val.multiDrawIndirect)},
									LabelValuePair{"Draw indirect first instance"s, bool(val.drawIndirectFirstInstance)},
									LabelValuePair{"Depth clamp"s, bool(val.depthClamp)},
									LabelValuePair{"Depth bias clamp"s, bool(val.depthBiasClamp)},
									LabelValuePair{"Fill mode non solid"s, bool(val.fillModeNonSolid)},
									LabelValuePair{"Depth bounds"s, bool(val.depthBounds)},
									LabelValuePair{"Wide lines"s, bool(val.wideLines)},
									LabelValuePair{"Large points"s, bool(val.largePoints)},
									LabelValuePair{"Alpha to one"s, bool(val.alphaToOne)},
									LabelValuePair{"Multi viewport"s, bool(val.multiViewport)},
									LabelValuePair{"Sampler anisotropy"s, bool(val.samplerAnisotropy)},
									LabelValuePair{"Texture compression ETC2"s, bool(val.textureCompressionETC2)},
									LabelValuePair{"Texture compression ASTC_LDR"s, bool(val.textureCompressionASTC_LDR)},
									LabelValuePair{"Texture compression BC"s, bool(val.textureCompressionBC)},
									LabelValuePair{"Occlusion query precise"s, bool(val.occlusionQueryPrecise)},
									LabelValuePair{"Pipeline statistics query"s, bool(val.pipelineStatisticsQuery)},
									LabelValuePair{"Vertex pipeline stores and atomics"s, bool(val.vertexPipelineStoresAndAtomics)},
									LabelValuePair{"Fragment stores and atomics"s, bool(val.fragmentStoresAndAtomics)},
									LabelValuePair{"Shader tessellation and geometry point size"s, bool(val.shaderTessellationAndGeometryPointSize)},
									LabelValuePair{"Shader image gather extended"s, bool(val.shaderImageGatherExtended)},
									LabelValuePair{"Shader storage image extended formats"s, bool(val.shaderStorageImageExtendedFormats)},
									LabelValuePair{"Shader storage image multisample"s, bool(val.shaderStorageImageMultisample)},
									LabelValuePair{"Shader storage image read without format"s, bool(val.shaderStorageImageReadWithoutFormat)},
									LabelValuePair{"Shader storage image write without format"s, bool(val.shaderStorageImageWriteWithoutFormat)},
									LabelValuePair{"Shader uniform buffer array dynamic indexing"s, bool(val.shaderUniformBufferArrayDynamicIndexing)},
									LabelValuePair{"Shader sampled image array dynamic indexing"s, bool(val.shaderSampledImageArrayDynamicIndexing)},
									LabelValuePair{"Shader storage buffer array dynamic indexing"s, bool(val.shaderStorageBufferArrayDynamicIndexing)},
									LabelValuePair{"Shader storage image array dynamic indexing"s, bool(val.shaderStorageImageArrayDynamicIndexing)},
									LabelValuePair{"Shader clip distance"s, bool(val.shaderClipDistance)},
									LabelValuePair{"Shader cull distance"s, bool(val.shaderCullDistance)},
									LabelValuePair{"Shader float64"s, bool(val.shaderFloat64)},
									LabelValuePair{"Shader int64"s, bool(val.shaderInt64)},
									LabelValuePair{"Shader int16"s, bool(val.shaderInt16)},
									LabelValuePair{"Shader resource residency"s, bool(val.shaderResourceResidency)},
									LabelValuePair{"Shader resource min LoD"s, bool(val.shaderResourceMinLod)},
									LabelValuePair{"Sparse binding"s, bool(val.sparseBinding)},
									LabelValuePair{"Sparse residency buffer"s, bool(val.sparseResidencyBuffer)},
									LabelValuePair{"Sparse residency image 2D"s, bool(val.sparseResidencyImage2D)},
									LabelValuePair{"Sparse residency image 3D"s, bool(val.sparseResidencyImage3D)},
									LabelValuePair{"Sparse residency 2 samples"s, bool(val.sparseResidency2Samples)},
									LabelValuePair{"Sparse residency 4 samples"s, bool(val.sparseResidency4Samples)},
									LabelValuePair{"Sparse residency 8 samples"s, bool(val.sparseResidency8Samples)},
									LabelValuePair{"Sparse residency 16 samples"s, bool(val.sparseResidency16Samples)},
									LabelValuePair{"Sparse residency aliased"s, bool(val.sparseResidencyAliased)},
									LabelValuePair{"Variable multisample rate"s, bool(val.variableMultisampleRate)},
									LabelValuePair{"Inherited queries"s, bool(val.inheritedQueries)});
}
inline auto getFormatString(vk::PhysicalDeviceMemoryProperties const& val)
{
	auto result = getLabelValuePairsString(LabelValuePair{"Memory type count"s, val.memoryTypeCount}) + ": "s;
	for (auto const& type : val.memoryTypes)
	{
		result += toString(type) + ", "s;
	}
	result += getLabelValuePairsString(LabelValuePair{"Memory heap count"s, val.memoryHeapCount}) + ": "s;
	for (auto const& heap : val.memoryHeaps)
	{
		if (heap.size != 0)
		{
			result += toString(heap) + ", "s;
		}
	}
	result[result.size() - 2] = ' ';
	return result;
}
inline auto getFormatString(vk::QueueFamilyProperties const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Queue type"s, val.queueFlags},
									LabelValuePair{"Queue count"s, val.queueCount},
									LabelValuePair{"Timestamp valid bits"s, val.timestampValidBits},
									LabelValuePair{"Min image transfer granularity"s, val.minImageTransferGranularity});
}
inline auto getFormatString(vk::SurfaceCapabilitiesKHR const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Min image count"s, val.minImageCount},
									LabelValuePair{"Max image count"s, val.maxImageCount},
									LabelValuePair{"Current extent"s, val.currentExtent},
									LabelValuePair{"Min image extent"s, val.minImageExtent},
									LabelValuePair{"Max image extent"s, val.maxImageExtent},
									LabelValuePair{"Max image array layers"s, val.maxImageArrayLayers},
									LabelValuePair{"Supported transforms"s, val.supportedTransforms},
									LabelValuePair{"Current transform"s, val.currentTransform},
									LabelValuePair{"Supported composite alpha"s, val.supportedCompositeAlpha},
									LabelValuePair{"Supported usage flags"s, val.supportedUsageFlags});
}
inline auto getFormatString(vk::SurfaceFormatKHR const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Format"s, val.format}, LabelValuePair{"Color space"s, val.colorSpace});
}
inline auto getFormatString(vk::PresentModeKHR const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Mode"s, val});
}
template<class T>
inline auto getFormatString(std::vector<T> const& vals)
{
	return toString<T>() + (vals.size() != 1 ? "s"s : ""s);
}
template<class T>
inline auto getFormatString(std::vector<vk::ExtensionProperties> const& vals)
{
	return toString<T>() + " extension"s + (vals.size() != 1 ? "s"s : ""s);
}
template <class T>
inline auto getFormatString(std::vector<char const*> const& vals)
{
	return toString<T>() + (vals.size() != 1 ? "s"s : ""s);
}
template<class T, class U>
inline auto getFormatString(std::vector<char const*> const& vals)
{
	return toString<T, U>() + (vals.size() != 1 ? "s"s : ""s);
}

//get string for total vector element count
template<class T>
inline auto getTotalString(std::vector<T> const& vals, std::string const& title = {})
{
	return std::vformat("{} "s + getFormatString(vals) + " "s + title + ":\n"s, std::make_format_args(vals.size()));
}
template<class T>
inline auto getTotalString(std::vector<T> const& vals, std::string const& title, std::string const& resourceName)
{
	return std::vformat("{} "s + resourceName + " "s + title + ":\n"s, std::make_format_args(vals.size()));
}

//print out contents of vector
template<class T>
inline std::ostream& operator<<(std::ostream& ostr, std::vector<T> const& values)
{
	for (auto const& val : values)
	{
		ostr << getFormatString(val) << "\n"s;
	}
	return ostr;
}