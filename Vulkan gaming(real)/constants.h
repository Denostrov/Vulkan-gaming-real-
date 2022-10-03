#pragma once

#include <array>

static constexpr std::array<char const*, 1> VALIDATION_LAYERS{"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
static constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif