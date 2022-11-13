#include "Text.h"

#include "ObjectPool.h"

Text::Text(std::string const& text, Font const& font, glm::vec3 const& position)
{
	letterQuads.reserve(text.size());
	auto currentX = position.x;
	uint32_t cellXCount = 256 / font.cellWidth;
	uint32_t cellYCount = 256 / font.cellHeight;
	for (unsigned char c : text)
	{
		letterQuads.push_back(0);
		c = std::max(c, font.startChar) - font.startChar;
		float xOffset = (c % cellXCount) * font.cellWidth / 256.0f;
		float yOffset = (c / cellXCount) * font.cellHeight / 256.0f;
		letterQuads[letterQuads.size() - 1] = ObjectPools::quads.add(QuadComponent(glm::vec3(currentX, position.y, position.z), font.scale,
											 glm::vec2(xOffset, yOffset), glm::vec2(font.cellWidth / 256.0f, font.cellHeight / 256.0f)), &letterQuads[letterQuads.size() - 1]);
		currentX += font.scale;
	}
}

Text::~Text()
{
	for (auto quad : letterQuads)
	{
		ObjectPools::quads.remove(quad);
	}
}

void Text::shift(glm::vec3 const& shift)
{
	for (auto quad : letterQuads)
	{
		auto quadData = ObjectPools::quads.data() + quad;
		quadData->setPosition(quadData->getPosition() + shift);
	}
}

TextBox::TextBox(glm::vec3 const& position, glm::vec2 const& size, Font const& font)
	:position(position), size(size), font(font)
{}

void TextBox::addText(std::string const& text, uint64_t lifetime)
{
	uint64_t rowChars = static_cast<uint64_t>(size.x / font.scale);
	uint64_t currentCharIndex = 0;
	while (currentCharIndex < text.size())
	{
		glm::vec3 rowPosition = glm::vec3(position.x, position.y + currentRow * font.scale, position.z);
		if (currentCharIndex + rowChars > text.size())
		{
			contents.push_back(Pair{std::make_unique<Text>(std::string(text.begin() + currentCharIndex, text.end()), font, rowPosition), lifetime});
			currentCharIndex = text.size();
		}
		else
		{
			contents.push_back(Pair{std::make_unique<Text>(std::string(text.begin() + currentCharIndex, text.begin() + currentCharIndex + rowChars), font, rowPosition),
									lifetime});
			currentCharIndex += rowChars;
		}
		currentRow++;
	}
}

void TextBox::update()
{
	for (uint64_t i = 0; i < contents.size(); i++)
	{
		if (contents[i].second == 0)
		{
			contents.erase(contents.begin() + i);
			i--;
			shiftRows();
			continue;
		}
		contents[i].second--;
	}
}

void TextBox::shiftRows()
{
	for (auto& [text, lifetime] : contents)
	{
		text->shift(glm::vec3(0.0f, -font.scale, 0.0f));
	}
	currentRow--;
}
