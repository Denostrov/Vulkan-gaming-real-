#include "Font.h"

#include <fstream>

using namespace nlohmann;

Font::Font(std::string const& fontInfoFilename)
{
	std::ifstream fontInfoFile{fontInfoFilename};
	assert(fontInfoFile && "couldn't open font info file");
	json fontInfo = json::parse(fontInfoFile, nullptr, false);

	bitmapWidth = fontInfo["bitmapWidth"];
	bitmapHeight = fontInfo["bitmapHeight"];
	scale = fontInfo["scale"];
	startChar = fontInfo["startChar"];
	cellWidth = fontInfo["cellWidth"];
	cellHeight = fontInfo["cellHeight"];
	charWidth = fontInfo["charWidth"];
	charHeight = fontInfo["charHeight"];
}

glm::vec2 Font::getCharOffset(unsigned char c) const
{
	c = std::max(c, startChar) - startChar;
	uint32_t cellXCount = bitmapWidth / cellWidth;
	uint32_t cellYCount = bitmapHeight / cellHeight;
	float xOffset = (c % cellXCount) * (float)cellWidth / bitmapWidth;
	float yOffset = (c / cellXCount) * (float)cellHeight / bitmapHeight;
	return glm::vec2(xOffset, yOffset);
}

glm::vec2 Font::getCharTextureScale() const
{
	float xScale = (float)charWidth / bitmapWidth;
	float yScale = (float)charHeight / bitmapHeight;
	return glm::vec2(xScale, yScale);
}