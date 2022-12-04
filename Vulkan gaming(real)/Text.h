#pragma once

#include "constants.h"
#include "helpers.h"
#include "QuadComponent.h"
#include "Font.h"

class Text
{
public:
	Text(std::string const& text, Font const& font, glm::vec3 const& position);
	~Text();

	void shift(glm::vec3 const& shift);

private:
	std::vector<std::size_t> letterQuads;
};

class TextBox
{
public:
	TextBox(glm::vec3 const& position, glm::vec2 const& size, Font const& font);

	void addText(std::string const& text, uint64_t lifetime);
	void update();

private:
	void shiftRows();
	uint64_t currentRow = 0;

	std::vector<Pair<std::unique_ptr<Text>, uint64_t>> contents;
	glm::vec3 position;
	glm::vec2 size;
	Font font;
};