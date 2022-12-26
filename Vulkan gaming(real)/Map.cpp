#include "Map.h"
#include "ObjectPool.h"

#include <random>

Map::Map(size_t width, size_t height, Font const& font)
	:width{width}, height{height}, position{-1.0f, -15.0f/16.0f, -0.1f}, scale{2.0f, 1.0f + 15.0f/16.0f}, font{font}, cells(width * height),
	adjacencyOffsets{{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}
{
	cellQuads.resize(width * height);
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			glm::vec3 quadPosition{scale.x / width * j + position.x, scale.y / height * i + position.y, position.z};
			glm::vec2 quadScale{scale.x / width, scale.y / height};
			ObjectPools::quads.add(QuadComponent(quadPosition, quadScale, font.getCharOffset('#'), font.getCharTextureScale()), &cellQuads[i * width + j]);
		}
	}
	for (size_t i = 0; i < 50; i++)
	{
		cells[i].mined = true;
	}
	std::random_device rd;
	std::minstd_rand gen(rd());
	std::shuffle(cells.begin(), cells.end(), gen);
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
	int64_t xIndex = static_cast<int64_t>(std::floor((xPos - position.x) / scale.x * width));
	int64_t yIndex = static_cast<int64_t>(std::floor((yPos - position.y) / scale.y * height));
	if (isIndexValid(xIndex, yIndex) && !cells[xIndex + yIndex * width].uncovered)
	{
		if (leftButton) pressCell(xIndex, yIndex);
		else markCell(xIndex, yIndex);
	}
}

void Map::pressCell(size_t xIndex, size_t yIndex)
{
	uint8_t mineCount = '0';
	for (auto offset : adjacencyOffsets)
	{
		int64_t xAdjacent = xIndex + offset.first;
		int64_t yAdjacent = yIndex + offset.second;
		if (isIndexValid(xAdjacent, yAdjacent) && cells[xAdjacent + width * yAdjacent].mined)
		{
			mineCount++;
		}
	}

	cells[xIndex + yIndex * width].uncovered = true;

	if (cells[xIndex + yIndex * width].mined) mineCount = 'X';
	else if (mineCount == '0')
	{
		mineCount = ' ';
		for (auto offset : adjacencyOffsets)
		{
			int64_t xAdjacent = xIndex + offset.first;
			int64_t yAdjacent = yIndex + offset.second;
			if (isIndexValid(xAdjacent, yAdjacent) && !cells[xAdjacent + width * yAdjacent].uncovered)
			{
				pressCell(xAdjacent, yAdjacent);
			}
		}
	}

	changeCellQuad(xIndex, yIndex, mineCount);
}

void Map::markCell(size_t xIndex, size_t yIndex)
{
	changeCellQuad(xIndex, yIndex, '!');
}

void Map::changeCellQuad(size_t xIndex, size_t yIndex, uint8_t newQuad)
{
	ObjectPools::quads.remove(cellQuads[xIndex + yIndex * width]);

	glm::vec3 quadPosition{ scale.x / width * xIndex + position.x, scale.y / height * yIndex + position.y, position.z };
	glm::vec2 quadScale{ scale.x / width, scale.y / height };
	ObjectPools::quads.add(QuadComponent(quadPosition, quadScale, font.getCharOffset(newQuad), font.getCharTextureScale()), &cellQuads[yIndex * width + xIndex]);
}

bool Map::isIndexValid(int64_t xIndex, int64_t yIndex)
{
	return xIndex >= 0 && xIndex < (int64_t)width && yIndex >= 0 && yIndex < (int64_t)height;
}


