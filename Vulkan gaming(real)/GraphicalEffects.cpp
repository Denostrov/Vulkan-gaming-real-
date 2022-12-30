#include "GraphicalEffects.h"

ColorFlash::ColorFlash(Font const& font)
	:font(font)
{}

ColorFlash::~ColorFlash()
{
	if (currentTimer < effectDuration)
	{
		ObjectPools::quads.remove(quad);
	}
}

void ColorFlash::start(glm::vec3 color, double duration)
{
	assert(duration > 0.0 && "ColorFlash with invalid timer");

	if (currentTimer < effectDuration)
	{
		ObjectPools::quads.remove(quad);
	}

	this->currentTimer = 0.0;
	this->color = color;
	this->effectDuration = duration;
	ObjectPools::quads.add(QuadComponent({ -1.0f, -1.0f, -0.05f }, { 2.0f, 2.0f }, font.getCharOffset(29), font.getCharTextureScale(),
		{ 0.0f, 0.0f, 0.0f, 0.0f }), &quad);
}

void ColorFlash::update()
{
	if (currentTimer < effectDuration)
	{
		currentTimer += TIME_STEP;
		if (currentTimer >= effectDuration)
		{
			ObjectPools::quads.remove(quad);
		}
		else
		{
			glm::vec4 newColor;
			if (currentTimer < effectDuration * 0.5)
			{
				newColor = glm::vec4(color, currentTimer / (effectDuration * 0.5));
			}
			else
			{
				newColor = glm::vec4(color, 1.0 - (currentTimer - effectDuration * 0.5) / (effectDuration * 0.5));
			}
			
			(ObjectPools::quads.data() + quad)->setColor(newColor);
		}
	}
}
