#include "QuadComponent.h"

QuadComponent::QuadComponent()
	:instanceData{}
{}

QuadComponent::QuadComponent(glm::vec3 position, float scale, glm::vec2 texOffset, glm::vec2 texScale)
	:instanceData{glm::vec4(position, scale), glm::vec4(texOffset, texScale)}
{}

glm::vec3 QuadComponent::getPosition() const
{
	return glm::vec3(instanceData.positionScale.x, instanceData.positionScale.y, instanceData.positionScale.z);
}

void QuadComponent::setPosition(glm::vec3 const& newPosition)
{
	instanceData.positionScale = glm::vec4(newPosition, instanceData.positionScale.w);
}
