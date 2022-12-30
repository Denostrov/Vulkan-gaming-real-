#pragma once

#include "constants.h"
#include "ObjectPool.h"
#include "Font.h"

class ColorFlash
{
public:
	ColorFlash(Font const& font);
	~ColorFlash();

	void start(glm::vec3 color, double duration);
	void update();

private:
	Font font;
	glm::vec3 color;
	double effectDuration{};
	double currentTimer{};

	size_t quad{};
};