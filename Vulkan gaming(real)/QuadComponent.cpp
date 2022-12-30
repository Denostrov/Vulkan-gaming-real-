#include "QuadComponent.h"

QuadComponent::QuadComponent()
	:instanceData{}
{}

QuadComponent::QuadComponent(glm::vec3 position, glm::vec2 scale, glm::vec2 texOffset, glm::vec2 texScale, glm::vec4 color)
	:instanceData{color, position, scale, glm::vec4(texOffset, texScale)}
{}
