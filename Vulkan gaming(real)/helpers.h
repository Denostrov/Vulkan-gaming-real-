#pragma once

#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <utility>

#include "logging.h"

template<class Vec, class Func>
inline bool checkVectorContainsString(Vec vector, Func vecStrAccess, char const* str)
{
	return std::find_if(vector.begin(), vector.end(),
		[&](auto const& p) { return std::strcmp(vecStrAccess(p), str); }) != vector.end();
}

inline std::vector<char> readFile(std::string const& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	errorFatal(bool(file), "couldn't open file: "s + filename);

	auto fileSize = std::streamoff(file.tellg());
	errorFatal(fileSize >= 0, "filesize is 0: "s + filename);

	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	return buffer;
}

inline uint32_t xorshift32(uint32_t& state)
{
	uint32_t res = state;
	res ^= res << 13;
	res ^= res >> 17;
	res ^= res << 5;
	return state = res;
}

inline uint32_t xorshift32c(uint32_t state)
{
	state ^= state << 13;
	state ^= state >> 17;
	state ^= state << 5;
	return state;
}

template<class First, class Second>
struct Pair
{
	First first;
	Second second;
};