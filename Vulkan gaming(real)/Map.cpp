#include "Map.h"
#include "ObjectPool.h"

#include <random>

Map::Map(size_t width, size_t height, Font const& font)
	:width{width}, height{height}, position{-1.0f, -15.0f/16.0f, -0.1f}, scale{2.0f, 1.0f + 15.0f/16.0f}, font{font}, mines(width * height, false)
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
		mines[i] = true;
	}
	std::random_device rd;
	std::minstd_rand gen(rd());
	std::shuffle(mines.begin(), mines.end(), gen);
}

Map::~Map()
{
	for (auto quad : cellQuads)
	{
		ObjectPools::quads.remove(quad);
	}
}

void Map::onMousePressed(double xPos, double yPos)
{
	int64_t xIndex = static_cast<int64_t>(std::floor((xPos - position.x) / scale.x * width));
	int64_t yIndex = static_cast<int64_t>(std::floor((yPos - position.y) / scale.y * height));
	if (xIndex >= 0 && xIndex < (int64_t)width && yIndex >= 0 && yIndex < (int64_t)height)
	{
		updateCell(xIndex, yIndex);
	}
}

void Map::updateCell(size_t xIndex, size_t yIndex)
{
	auto mineCount = '0';
	int64_t upY = yIndex - 1;
	int64_t downY = yIndex + 1;
	int64_t leftX = xIndex - 1;
	int64_t rightX = xIndex + 1;
	if (leftX >= 0 && mines[leftX + yIndex * width]) mineCount++;
	if (upY >= 0 && mines[xIndex + upY * width]) mineCount++;
	if (leftX >= 0 && upY >= 0 && mines[leftX + upY * width]) mineCount++;
	if (rightX < (int64_t)width && mines[rightX + yIndex * width]) mineCount++;
	if (rightX < (int64_t)width && upY >= 0 && mines[rightX + upY * width]) mineCount++;
	if (downY < (int64_t)height && mines[xIndex + downY * width]) mineCount++;
	if (leftX >= 0 && downY < (int64_t)height && mines[leftX + downY * width]) mineCount++;
	if (rightX < (int64_t)width && downY < (int64_t)height && mines[rightX + downY * width]) mineCount++;

	if (mineCount == '0')
	{
		mineCount = ' ';
	}
	if (mines[xIndex + yIndex * width]) mineCount = 'X';

	ObjectPools::quads.remove(cellQuads[xIndex + yIndex * width]);

	glm::vec3 quadPosition{scale.x / width * xIndex + position.x, scale.y / height * yIndex + position.y, position.z};
	glm::vec2 quadScale{scale.x / width, scale.y / height};
	ObjectPools::quads.add(QuadComponent(quadPosition, quadScale, font.getCharOffset(mineCount), font.getCharTextureScale()), &cellQuads[yIndex * width + xIndex]);
}

void Map::updateNeighbors(size_t xIndex, size_t yIndex)
{
	int64_t upY = yIndex - 1;
	int64_t downY = yIndex + 1;
	int64_t leftX = xIndex - 1;
	int64_t rightX = xIndex + 1;
}


