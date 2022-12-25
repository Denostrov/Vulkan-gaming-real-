#pragma once

#include <deque>

#include "constants.h"
#include "Text.h"

class Map
{
	struct Cell
	{
		bool mined = false;
		bool uncovered = false;
	};

public:
	Map(size_t width, size_t height, Font const& font);
	~Map();

	void onMousePressed(double xPos, double yPos);
	void pressCell(size_t xIndex, size_t yIndex);
	void updateNeighbors(size_t xIndex, size_t yIndex);

private:
	bool isIndexValid(int64_t xIndex, int64_t yIndex);

	size_t width;
	size_t height;
	glm::vec3 position;
	glm::vec2 scale;
	Font font;

	std::vector<std::pair<int64_t, int64_t>> adjacencyOffsets;

	std::vector<size_t> cellQuads;
	std::vector<Cell> cells;
};