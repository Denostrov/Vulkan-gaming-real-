#pragma once

#include "constants.h"
#include "QuadComponent.h"

class Text
{
public:
	Text(std::string const& text, glm::vec3 const& position);
	~Text();

private:
	std::vector<std::size_t> letterQuads;
};