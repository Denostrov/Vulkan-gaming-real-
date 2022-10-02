#pragma once

#include <iostream>
#include <format>
#include <string>
#include <string_view>

template<class... Args>
void printConsole(std::string_view format, Args&&... args)
{
	std::cout << std::vformat(format, std::make_format_args(std::forward<Args>(args)...));
}