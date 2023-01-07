#include "Map.h"
#include "ObjectPool.h"

#include <random>

Map::Map(size_t width, size_t height, size_t mineCount, Font const& font, std::vector<RefWrapper<Observer>> const& observers)
	:notifier{ NotifierType::eMap, observers }, width{ width }, height{ height },
	position{ -1.0f, -14.0f / 16.0f, -0.1f }, scale{ 2.0f, 1.0f + 14.0f / 16.0f }, font{ font }, cells(width* height),
	adjacencyOffsets{ {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} },
	coveredCellCount{ width * height }, currentState{ State::ePreparing }
{
	populateMines(mineCount);
}

Map::~Map()
{
	for (auto quad : cellQuads)
	{
		ObjectPools::quads.remove(quad);
	}
}

void Map::onMousePressed(double xPos, double yPos, bool leftButton)
{
	if (inputBlocked) return;

	int64_t xIndex = static_cast<int64_t>(std::floor((xPos - position.x) / scale.x * width));
	int64_t yIndex = static_cast<int64_t>(std::floor((yPos - position.y) / scale.y * height));
	if (!isIndexValid(xIndex, yIndex)) return;

	auto& clickedCell = getCellAtIndex(xIndex, yIndex);
	if (clickedCell.state == CellState::eCovered)
	{
		if (leftButton) pressCell(xIndex, yIndex);
		else markCell(xIndex, yIndex);
	}
	else if (clickedCell.state == CellState::eMarked)
	{
		if (!leftButton) unmarkCell(xIndex, yIndex);
	}
	else
	{
		checkCell(xIndex, yIndex);
		inputBlocked = true;
	}
}

void Map::onMouseReleased()
{
	if (inputBlocked && currentState == State::ePlaying)
	{
		uncheckCell(checkedCellIndices.first, checkedCellIndices.second);
		inputBlocked = false;
	}
}

void Map::reset()
{
	for (auto quad : cellQuads)
	{
		ObjectPools::quads.remove(quad);
	}
	cells = std::vector<Cell>(width * height);
	coveredCellCount = width * height;
	markedCellCount = 0;

	populateMines((randInt() % (width * height / 4)) + 1);

	changeState(State::ePreparing);
}

void Map::populateMines(size_t newMineCount)
{
	cellQuads.resize(width * height);
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			glm::vec3 quadPosition{ scale.x / width * j + position.x, scale.y / height * i + position.y, position.z };
			glm::vec2 quadScale{ scale.x / width, scale.y / height };
			ObjectPools::quads.add(QuadComponent(quadPosition, quadScale, font.getCharOffset('#'), font.getCharTextureScale()), &cellQuads[i * width + j]);
		}
	}

	newMineCount = std::min(newMineCount, cells.size() - 1);
	mineCount = newMineCount;
	coveredCellCount -= newMineCount;
	for (size_t i = 0; i < newMineCount; i++)
	{
		cells[i].mined = true;
	}
	std::random_device rd;
	std::minstd_rand gen(rd());
	std::shuffle(cells.begin(), cells.end(), gen);
}

void Map::pressCell(size_t xIndex, size_t yIndex)
{
	uint8_t adjacentMinesCount = countAdjacentMines(xIndex, yIndex);

	auto& pressedCell = getCellAtIndex(xIndex, yIndex);
	pressedCell.state = CellState::eUncovered;

	if (pressedCell.mined)
	{
		adjacentMinesCount = 'X';
		changeState(State::eLost);
	}
	else if (adjacentMinesCount == '0')
	{
		adjacentMinesCount = ' ';
		pressAdjacentCells(xIndex, yIndex);
	}

	if (adjacentMinesCount != 'X')
	{
		coveredCellCount--;
		if (coveredCellCount == 0) changeState(State::eWon);
		else if (currentState == State::ePreparing) changeState(State::ePlaying);
	}

	changeCellQuad(xIndex, yIndex, adjacentMinesCount);
}

void Map::markCell(size_t xIndex, size_t yIndex)
{
	getCellAtIndex(xIndex, yIndex).state = CellState::eMarked;
	markedCellCount++;
	changeCellQuad(xIndex, yIndex, '!');
}

void Map::unmarkCell(size_t xIndex, size_t yIndex)
{
	getCellAtIndex(xIndex, yIndex).state = CellState::eCovered;
	markedCellCount--;
	changeCellQuad(xIndex, yIndex, '#');
}

void Map::checkCell(size_t xIndex, size_t yIndex)
{
	for (auto&& [xOffset, yOffset] : adjacencyOffsets)
	{
		int64_t xAdjacent = xIndex + xOffset;
		int64_t yAdjacent = yIndex + yOffset;
		if (isIndexValid(xAdjacent, yAdjacent) && getCellAtIndex(xAdjacent, yAdjacent).state == CellState::eCovered)
		{
			changeCellQuad(xAdjacent, yAdjacent, '?');
		}
	}
	checkedCellIndices = { xIndex, yIndex };
}

void Map::uncheckCell(size_t xIndex, size_t yIndex)
{
	if (countAdjacentMarks(xIndex, yIndex) == countAdjacentMines(xIndex, yIndex))
	{
		pressAdjacentCells(xIndex, yIndex);
	}
	else
	{
		for (auto&& [xOffset, yOffset] : adjacencyOffsets)
		{
			int64_t xAdjacent = xIndex + xOffset;
			int64_t yAdjacent = yIndex + yOffset;
			if (isIndexValid(xAdjacent, yAdjacent) && getCellAtIndex(xAdjacent, yAdjacent).state == CellState::eCovered)
			{
				changeCellQuad(xAdjacent, yAdjacent, '#');
			}
		}
	}
}

void Map::pressAdjacentCells(size_t xIndex, size_t yIndex)
{
	for (auto&& [xOffset, yOffset] : adjacencyOffsets)
	{
		int64_t xAdjacent = xIndex + xOffset;
		int64_t yAdjacent = yIndex + yOffset;
		if (isIndexValid(xAdjacent, yAdjacent) && getCellAtIndex(xAdjacent, yAdjacent).state == CellState::eCovered)
		{
			pressCell(xAdjacent, yAdjacent);
		}
	}
}

Map::Cell& Map::getCellAtIndex(size_t xIndex, size_t yIndex)
{
	return cells[xIndex + width * yIndex];
}

void Map::changeCellQuad(size_t xIndex, size_t yIndex, uint8_t newQuad)
{
	ObjectPools::quads.remove(cellQuads[xIndex + yIndex * width]);

	glm::vec3 cellColor{ 1.0f, 1.0f, 1.0f };
	switch (newQuad)
	{
	case 'X':
		cellColor = { 1.0f, 0.0f, 1.0f };
		break;
	case '!':
		cellColor = { 1.0f, 0.5f, 0.0f };
		break;
	case '1':
		cellColor = { 0.0f, 0.0f, 1.0f };
		break;
	case '2':
		cellColor = { 0.0f, 0.5f, 0.0f };
		break;
	case '3':
		cellColor = { 1.0f, 0.0f, 0.0f };
		break;
	case '4':
		cellColor = { 0.0f, 0.0f, 0.5f };
		break;
	case '5':
		cellColor = { 0.5f, 0.0f, 0.0f };
		break;
	case '6':
		cellColor = { 0.0f, 0.5f, 0.5f };
		break;
	case '7':
		cellColor = { 0.5f, 0.5f, 0.0f };
		break;
	case '8':
		cellColor = { 0.5f, 0.5f, 0.5f };
		break;
	default:
		break;
	}

	glm::vec3 quadPosition{ scale.x / width * xIndex + position.x, scale.y / height * yIndex + position.y, position.z };
	glm::vec2 quadScale{ scale.x / width, scale.y / height };
	ObjectPools::quads.add(QuadComponent(quadPosition, quadScale, font.getCharOffset(newQuad), font.getCharTextureScale(), glm::vec4(cellColor, 1.0f)), &cellQuads[yIndex * width + xIndex]);
}

bool Map::isIndexValid(int64_t xIndex, int64_t yIndex)
{
	return xIndex >= 0 && xIndex < (int64_t)width&& yIndex >= 0 && yIndex < (int64_t)height;
}

void Map::changeState(State newState)
{
	inputBlocked = newState == State::eLost || newState == State::eWon;
	currentState = newState;
	notifier.notify((size_t)newState);
}


