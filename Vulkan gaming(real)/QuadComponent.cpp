#include "QuadComponent.h"

QuadComponent::QuadComponent()
	:instanceData{}
{}

QuadComponent::QuadComponent(glm::vec3 position, float scale, glm::vec2 texOffset, glm::vec2 texScale)
	:instanceData{glm::vec4(position, scale), glm::vec4(texOffset, texScale)}
{}