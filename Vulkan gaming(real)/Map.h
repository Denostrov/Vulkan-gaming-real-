#pragma once

#include <deque>

#include "constants.h"
#include "Text.h"

class Map
{
public:
	Map(size_t width, size_t height, Font const& font);
	~Map();

	void onMousePressed(double xPos, double yPos);
	void updateCell(size_t xIndex, size_t yIndex);

private:
	size_t width;
	size_t height;
	glm::vec3 position;
	glm::vec2 scale;
	Font font;

	std::vector<std::size_t> cellQuads;
	std::deque<bool> mines;
};