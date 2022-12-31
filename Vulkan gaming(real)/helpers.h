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

template<class Return, class... Args>
struct FreeFunction
{
	FreeFunction(Return(*func)(Args...))
		:func(func)
	{}

	Return operator()(Args... args) { return func(args...); }

	Return(*func)(Args...);
};

template<class Return, class Member, class... Args>
struct MemberFunction
{
	MemberFunction(Member& member, Return(Member::* func)(Args...))
		:member(member), func(func)
	{}

	Return operator()(Args... args) { return (member.*func)(args...); }

	Member& member;
	Return(Member::* func)(Args...);
};

template<class Reference>
struct RefWrapper
{
	template<class U, std::enable_if_t<!std::is_same_v<RefWrapper, std::decay_t<U>>, bool> = true>
	constexpr RefWrapper(U&& ref)
		:ref{std::forward<U>(ref)}
	{}

	constexpr RefWrapper(RefWrapper<Reference> const& other) noexcept
		:ref{other.ref}
	{}

	constexpr Reference& get() const noexcept { return ref.get(); }

	std::reference_wrapper<Reference> ref;
};

template<class Reference>
inline bool operator==(RefWrapper<Reference> const& lhs, RefWrapper<Reference> const& rhs)
{
	return std::addressof(lhs.ref.get()) == std::addressof(rhs.ref.get());
}

template<class Reference>
constexpr RefWrapper<Reference> refWrap(Reference& ref) noexcept
{
	return RefWrapper<Reference>(ref);
}
template<class Reference>
constexpr RefWrapper<Reference> refWrap(RefWrapper<Reference> ref) noexcept
{
	return ref;
}
template<class Reference>
void refWrap(Reference const&&) = delete;