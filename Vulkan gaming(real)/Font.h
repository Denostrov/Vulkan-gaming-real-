#pragma once

#include <json.hpp>

#include "constants.h"

class Font
{
public:
	Font(std::string const& fontInfoFilename);

	glm::vec2 getCharOffset(unsigned char c) const;
	glm::vec2 getCharTextureScale() const;

	uint32_t bitmapWidth;
	uint32_t bitmapHeight;
	float scale;
	uint8_t startChar;
	uint32_t cellWidth;
	uint32_t cellHeight;
	uint32_t charWidth;
	uint32_t charHeight;
};