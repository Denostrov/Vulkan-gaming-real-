#pragma once

#include "constants.h"

class QuadComponent
{
public:
	QuadComponent();
	QuadComponent(glm::vec3 position, float scale, glm::vec2 texOffset, glm::vec2 texScale);

private:
	InstanceVertex instanceData;
};