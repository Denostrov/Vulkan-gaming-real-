#pragma once

#include <iostream>
#include <format>
#include <string>
#include <string_view>

#include "constants.h"
#include "helpers.h"

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
	auto result = ""s;
	for (auto const& elem : arr)
	{
		result += elem;
	}
	return result;
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
	return toString(VkSampleCountFlags(flags));
}
inline std::string toString(bool val)
{
	return val ? "True"s : "False"s;
}
inline std::string toString(QueueFamilyIndices const& val)
{
	return std::vformat("{{Graphics: {}, Presentation: {}}}"sv, std::make_format_args(val.graphicsFamily, val.presentationFamily));
}
template<class T>
inline std::string toString() {}
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