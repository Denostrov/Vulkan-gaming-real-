#include "Text.h"

#include "ObjectPool.h"

Text::Text(std::string const& text, glm::vec3 const& position)
{
	letterQuads.reserve(text.size());
	auto currentX = position.x;
	for (unsigned char c : text)
	{
		letterQuads.push_back(0);
		c = std::max(c, (uint8_t)27) - 27;
		float xOffset = (c % 21) * 12.0f / 256.0f;
		float yOffset = (c / 21) * 20.0f / 256.0f;
		letterQuads[letterQuads.size() - 1] = ObjectPools::quads.add(QuadComponent(glm::vec3(currentX, position.y, position.z), 1.0f / 16.0f,
											 glm::vec2(xOffset, yOffset), glm::vec2(12.0f / 256.0f, 20.0f / 256.0f)), &letterQuads[letterQuads.size() - 1]);
		currentX += 1.0f / 16.0f;
	}
}

Text::~Text()
{
	for (auto quad : letterQuads)
	{
		ObjectPools::quads.remove(quad);
	}
}