#pragma once

#include "constants.h"
#include "Text.h"

template <class OnClick>
class Button
{
public:
	Button(glm::vec3 position, glm::vec2 scale, Font const& font, std::string const& text, OnClick const& onClick)
		:position{position}, scale{scale}, font{font}, text{text},
		textQuads(text, font, getTextPosition()), onClick(onClick)
	{
		ObjectPools::quads.add(QuadComponent(position, scale, font.getCharOffset(28), font.getCharTextureScale()), &borderQuad);
	}

	void onMousePressed(double xPos, double yPos)
	{
		if (xPos >= position.x && yPos >= position.y && xPos <= position.x + scale.x && yPos <= position.y + scale.y)
		{
			onClick();
		}
	}

private:
	glm::vec3 getTextPosition()
	{
		return glm::vec3(position.x + font.scale / 4.0f, position.y + scale.y / 2.0f - font.scale / 2.0f, position.z);
	}

	glm::vec3 position;
	glm::vec2 scale;
	Font font;
	std::string text;

	size_t borderQuad;
	Text textQuads;

	OnClick onClick;
};