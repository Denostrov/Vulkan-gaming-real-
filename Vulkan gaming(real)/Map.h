#pragma once

#include <deque>

#include "constants.h"
#include "Text.h"

class Map
{
	enum class CellState
	{
		eCovered, eUncovered, eMarked
	};
	struct Cell
	{
		bool mined = false;
		CellState state = CellState::eCovered;
	};

public:
	Map(size_t width, size_t height, Font const& font);
	~Map();

	void onMousePressed(double xPos, double yPos, bool leftButton);
	void onMouseReleased();

private:
	void pressCell(size_t xIndex, size_t yIndex);
	void markCell(size_t xIndex, size_t yIndex);
	void unmarkCell(size_t xIndex, size_t yIndex);
	void checkCell(size_t xIndex, size_t yIndex);
	void uncheckCell(size_t xIndex, size_t yIndex);

	void pressAdjacentCells(size_t xIndex, size_t yIndex);

	template<class Condition>
	uint8_t countAdjacentStates(size_t xIndex, size_t yIndex, Condition condition)
	{
		uint8_t stateCount = '0';
		for (auto&& [xOffset, yOffset] : adjacencyOffsets)
		{
			int64_t xAdjacent = xIndex + xOffset;
			int64_t yAdjacent = yIndex + yOffset;
			if (isIndexValid(xAdjacent, yAdjacent) && condition(xAdjacent, yAdjacent))
			{
				stateCount++;
			}
		}
		return stateCount;
	}
	uint8_t countAdjacentMines(size_t xIndex, size_t yIndex) 
	{
		return countAdjacentStates(xIndex, yIndex, [&](size_t xIndex, size_t yIndex) { return getCellAtIndex(xIndex, yIndex).mined; });
	}
	uint8_t countAdjacentMarks(size_t xIndex, size_t yIndex)
	{
		return countAdjacentStates(xIndex, yIndex, [&](size_t xIndex, size_t yIndex) { return getCellAtIndex(xIndex, yIndex).state == CellState::eMarked; });
	}

	Cell& getCellAtIndex(size_t xIndex, size_t yIndex);
	void changeCellQuad(size_t xIndex, size_t yIndex, uint8_t newQuad);

	bool isIndexValid(int64_t xIndex, int64_t yIndex);

	bool inputBlocked = false;
	std::pair<size_t, size_t> checkedCellIndices;

	size_t width;
	size_t height;
	glm::vec3 position;
	glm::vec2 scale;
	Font font;

	std::vector<std::pair<int64_t, int64_t>> adjacencyOffsets;

	std::vector<size_t> cellQuads;
	std::vector<Cell> cells;
};