#pragma once

#include "constants.h"

class QuadComponent
{
public:
	QuadComponent();
	QuadComponent(glm::vec3 position, glm::vec2 scale, glm::vec2 texOffset, glm::vec2 texScale, glm::vec3 color = {1.0f, 1.0f, 1.0f});

	glm::vec3 getPosition() const;
	void setPosition(glm::vec3 const& newPosition);

private:
	InstanceVertex instanceData;
};