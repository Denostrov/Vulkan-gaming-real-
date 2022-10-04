#pragma once

#include <string>

template<class Vec, class Func>
inline bool checkVectorContainsString(Vec vector, Func vecStrAccess, char const* str)
{
	return std::find_if(vector.begin(), vector.end(),
		[&](auto const& p) { return std::strcmp(vecStrAccess(p), str); }) != vector.end();
}