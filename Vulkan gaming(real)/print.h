#pragma once

#include <iostream>
#include <format>
#include <string>
#include <string_view>

#include "constants.h"

using namespace std::literals;

template<class... Args>
inline void formatPrint(std::ostream& ostr, std::string_view format, Args&&... args)
{
	ostr << std::vformat(format, std::make_format_args(std::forward<Args>(args)...));
}

template<class T>
struct LabelValuePair
{
	LabelValuePair(std::string const& label, T const& value)
		:label(label), value(value)
	{}

	std::string label;
	T value;
};

inline std::string getLabelValuePairsString() { return ""s; }
template<class T, class... Pairs>
inline std::string getLabelValuePairsString(LabelValuePair<T> const& pair, Pairs&&... pairs)
{
	return std::vformat("\t"s + pair.label + ": {}"s + (sizeof...(pairs) != 0 ? ","s : "\n"), std::make_format_args(pair.value)) +
		getLabelValuePairsString(std::forward<Pairs>(pairs)...);
}

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
inline auto getFormatString(std::vector<vk::ExtensionProperties> const& vals)
{
	return "Instance extension"s + (vals.size() != 1 ? "s"s : ""s);
}
inline auto getFormatString(std::vector<vk::LayerProperties> const& vals)
{
	return "Layer"s + (vals.size() != 1 ? "s"s : ""s);
}

template<class T>
inline auto getTotalString(std::vector<T> const& vals, std::string const& title = {})
{
	return std::vformat("{} "s + title + " "s + getFormatString(vals) + ":\n"s, std::make_format_args(vals.size()));
}
inline auto getTotalString(std::vector<char const*> const& vals, std::string const& title = {}, std::string const& resourceName = {})
{
	return std::vformat("{} "s + title + " "s + resourceName + ":\n"s, std::make_format_args(vals.size()));
}

template<class T>
inline std::ostream& operator<<(std::ostream& ostr, std::vector<T> const& values)
{
	for (auto const& val : values)
	{
		ostr << getFormatString(val);
	}
	return ostr;
}