#pragma once

#include <iostream>
#include <format>
#include <string>
#include <string_view>

template<class... Args>
inline void printConsole(std::string_view format, Args&&... args)
{
	std::cout << std::vformat(format, std::make_format_args(std::forward<Args>(args)...));
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

inline void printLabelValuePairs(std::ostream& ostr)
{}
template<class T, class... Pairs>
inline void printLabelValuePairs(std::ostream& ostr, LabelValuePair<T> const& pair, Pairs&&... pairs)
{
	using namespace std::literals;

	ostr << std::vformat("\t"s + pair.label + ": {}"s + (sizeof...(pairs) != 0 ? ","s : "\n"), std::make_format_args(pair.value));
	printLabelValuePairs(ostr, std::forward<Pairs>(pairs)...);
}