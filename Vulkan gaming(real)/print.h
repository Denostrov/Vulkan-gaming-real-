#pragma once

#include <iostream>
#include <format>
#include <string>
#include <string_view>

#include "constants.h"

using namespace std::literals;

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
inline std::string toString(vk::Bool32 val)
{
	return val == 0 ? "False" : "True";
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
	return std::vformat("\t"s + pair.label + ": {}"s + (sizeof...(pairs) != 0 ? ","s : ""s), std::make_format_args(pair.value)) +
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
inline auto getFormatString(vk::PhysicalDeviceProperties const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Device name"s, val.deviceName},
									LabelValuePair{"Device type"s, toString(val.deviceType)},
									LabelValuePair{"API version"s, val.apiVersion},
									LabelValuePair{"Driver version"s, val.driverVersion},
									LabelValuePair{"Vendor ID"s, val.vendorID},
									LabelValuePair{"Device ID"s, val.deviceID});
}
inline auto getFormatString(vk::PhysicalDeviceFeatures const& val)
{
	return getLabelValuePairsString(LabelValuePair{"Robust buffer access"s, toString(val.robustBufferAccess)},
									LabelValuePair{"Full draw index Uint32"s, toString(val.fullDrawIndexUint32)},
									LabelValuePair{"Image cube array"s, toString(val.imageCubeArray)},
									LabelValuePair{"Independent blend"s, toString(val.independentBlend)},
									LabelValuePair{"Geometry shader"s, toString(val.geometryShader)},
									LabelValuePair{"Tessellation shader"s, toString(val.tessellationShader)},
									LabelValuePair{"Sample rate shading"s, toString(val.sampleRateShading)},
									LabelValuePair{"Dual source blend"s, toString(val.dualSrcBlend)},
									LabelValuePair{"Logic operation"s, toString(val.logicOp)},
									LabelValuePair{"Multi draw indirect"s, toString(val.multiDrawIndirect)},
									LabelValuePair{"Draw indirect first instance"s, toString(val.drawIndirectFirstInstance)},
									LabelValuePair{"Depth clamp"s, toString(val.depthClamp)},
									LabelValuePair{"Depth bias clamp"s, toString(val.depthBiasClamp)},
									LabelValuePair{"Fill mode non solid"s, toString(val.fillModeNonSolid)},
									LabelValuePair{"Depth bounds"s, toString(val.depthBounds)},
									LabelValuePair{"Wide lines"s, toString(val.wideLines)},
									LabelValuePair{"Large points"s, toString(val.largePoints)},
									LabelValuePair{"Alpha to one"s, toString(val.alphaToOne)},
									LabelValuePair{"Multi viewport"s, toString(val.multiViewport)},
									LabelValuePair{"Sampler anisotropy"s, toString(val.samplerAnisotropy)},
									LabelValuePair{"Texture compression ETC2"s, toString(val.textureCompressionETC2)},
									LabelValuePair{"Texture compression ASTC_LDR"s, toString(val.textureCompressionASTC_LDR)},
									LabelValuePair{"Texture compression BC"s, toString(val.textureCompressionBC)},
									LabelValuePair{"Occlusion query precise"s, toString(val.occlusionQueryPrecise)},
									LabelValuePair{"Pipeline statistics query"s, toString(val.pipelineStatisticsQuery)},
									LabelValuePair{"Vertex pipeline stores and atomics"s, toString(val.vertexPipelineStoresAndAtomics)},
									LabelValuePair{"Fragment stores and atomics"s, toString(val.fragmentStoresAndAtomics)},
									LabelValuePair{"Shader tessellation and geometry point size"s, toString(val.shaderTessellationAndGeometryPointSize)},
									LabelValuePair{"Shader image gather extended"s, toString(val.shaderImageGatherExtended)},
									LabelValuePair{"Shader storage image extended formats"s, toString(val.shaderStorageImageExtendedFormats)},
									LabelValuePair{"Shader storage image multisample"s, toString(val.shaderStorageImageMultisample)},
									LabelValuePair{"Shader storage image read without format"s, toString(val.shaderStorageImageReadWithoutFormat)},
									LabelValuePair{"Shader storage image write without format"s, toString(val.shaderStorageImageWriteWithoutFormat)},
									LabelValuePair{"Shader uniform buffer array dynamic indexing"s, toString(val.shaderUniformBufferArrayDynamicIndexing)},
									LabelValuePair{"Shader sampled image array dynamic indexing"s, toString(val.shaderSampledImageArrayDynamicIndexing)},
									LabelValuePair{"Shader storage buffer array dynamic indexing"s, toString(val.shaderStorageBufferArrayDynamicIndexing)},
									LabelValuePair{"Shader storage image array dynamic indexing"s, toString(val.shaderStorageImageArrayDynamicIndexing)},
									LabelValuePair{"Shader clip distance"s, toString(val.shaderClipDistance)},
									LabelValuePair{"Shader cull distance"s, toString(val.shaderCullDistance)},
									LabelValuePair{"Shader float64"s, toString(val.shaderFloat64)},
									LabelValuePair{"Shader int64"s, toString(val.shaderInt64)},
									LabelValuePair{"Shader int16"s, toString(val.shaderInt16)},
									LabelValuePair{"Shader resource residency"s, toString(val.shaderResourceResidency)},
									LabelValuePair{"Shader resource min LoD"s, toString(val.shaderResourceMinLod)},
									LabelValuePair{"Sparse binding"s, toString(val.sparseBinding)},
									LabelValuePair{"Sparse residency buffer"s, toString(val.sparseResidencyBuffer)},
									LabelValuePair{"Sparse residency image 2D"s, toString(val.sparseResidencyImage2D)},
									LabelValuePair{"Sparse residency image 3D"s, toString(val.sparseResidencyImage3D)},
									LabelValuePair{"Sparse residency 2 samples"s, toString(val.sparseResidency2Samples)},
									LabelValuePair{"Sparse residency 4 samples"s, toString(val.sparseResidency4Samples)},
									LabelValuePair{"Sparse residency 8 samples"s, toString(val.sparseResidency8Samples)},
									LabelValuePair{"Sparse residency 16 samples"s, toString(val.sparseResidency16Samples)},
									LabelValuePair{"Sparse residency aliased"s, toString(val.sparseResidencyAliased)},
									LabelValuePair{"Variable multisample rate"s, toString(val.variableMultisampleRate)},
									LabelValuePair{"Inherited queries"s, toString(val.inheritedQueries)});
}
inline auto getFormatString(std::vector<vk::ExtensionProperties> const& vals)
{
	return "Instance extension"s + (vals.size() != 1 ? "s"s : ""s);
}
inline auto getFormatString(std::vector<vk::LayerProperties> const& vals)
{
	return "Layer"s + (vals.size() != 1 ? "s"s : ""s);
}
inline auto getFormatString(std::vector<vk::PhysicalDevice> const& vals)
{
	return "Physical device"s + (vals.size() != 1 ? "s"s : ""s);
}

//get string for total vector element count
template<class T>
inline auto getTotalString(std::vector<T> const& vals, std::string const& title = {})
{
	return std::vformat("{} "s + getFormatString(vals) + " "s + title + ":\n"s, std::make_format_args(vals.size()));
}
inline auto getTotalString(std::vector<char const*> const& vals, std::string const& title = {}, std::string const& resourceName = {})
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