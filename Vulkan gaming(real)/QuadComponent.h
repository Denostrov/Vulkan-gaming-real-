#pragma once

#include "constants.h"

class QuadComponent
{
public:
	QuadComponent();
	QuadComponent(glm::vec3 position, glm::vec2 scale, glm::vec2 texOffset, glm::vec2 texScale, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });

	glm::vec3 getPosition() const { return instanceData.position; }
	void setPosition(glm::vec3 const& newPosition) { instanceData.position = newPosition; }
	glm::vec4 getColor() const { return instanceData.color; }
	void setColor(glm::vec4 const& newColor) { instanceData.color = newColor; }

private:
	InstanceVertex instanceData;
};