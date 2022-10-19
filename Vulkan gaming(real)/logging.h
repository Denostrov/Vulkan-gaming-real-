#pragma once

#include <format>
#include <iostream>
#include <fstream>
#include <string>

#include "constants.h"

inline std::ofstream debugLog{"log.txt", std::ios::binary};
inline std::ofstream errorLog("errorLog.txt", std::ios::binary);

template<class T>
auto errorFatal(vk::ResultValue<T> const& rv, std::string const& message = {})
{
	if (rv.result != vk::Result::eSuccess)
	{
		errorLog << toString(rv.result) << ": "s << message << "\n"s;
		std::exit(-1);
	}
	return rv.value;
}

inline auto errorFatal(bool val, std::string const& message = {})
{
	if (!val)
	{
		errorLog << message << "\n"s;
		std::exit(-1);
	}
}

inline VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, VkDebugUtilsMessengerCallbackDataEXT const* callbackData, void* userData)
{
	using namespace std::literals;

	auto severity = ""s;
	severity += messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ? "Error "s : ""s;
	severity += messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ? "Warning "s : ""s;
	severity += messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT ? "Info "s : ""s;
	severity += messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT ? "Verbose "s : ""s;

	auto type = ""s;
	switch (messageType)
	{
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		type = "General "s;
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		type = "Performance "s;
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		type = "Validation "s;
		break;
	default:
		type = "Unknown "s;
		break;
	}

	auto printMessage = [&](auto& output)
	{
		output << std::format("Validation layer: {}{}{}\n"sv, severity, type, callbackData->pMessage);
	};

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		printMessage(errorLog);
	}
	else
	{
		printMessage(debugLog);
	}

	return VK_FALSE;
}