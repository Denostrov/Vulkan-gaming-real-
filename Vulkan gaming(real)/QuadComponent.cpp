#include "QuadComponent.h"

QuadComponent::QuadComponent()
	:instanceData{}
{}

QuadComponent::QuadComponent(glm::vec3 position, glm::vec2 scale, glm::vec2 texOffset, glm::vec2 texScale)
	:instanceData{position, scale, glm::vec4(texOffset, texScale)}
{}

glm::vec3 QuadComponent::getPosition() const
{
	return instanceData.position;
}

void QuadComponent::setPosition(glm::vec3 const& newPosition)
{
	instanceData.position = newPosition;
}
