#version 450

layout(binding = 0) uniform UniformBufferObject
{
	mat4 vp;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inColor;
layout(location = 3) in vec3 inTranslation;
layout(location = 4) in vec2 inScale;
layout(location = 5) in vec4 inTexOffsetScale;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec4 fragTexOffsetScale;

void main()
{
	gl_Position = ubo.vp * vec4(vec3(inPosition.x * inScale.x, inPosition.y * inScale.y, inPosition.z) + inTranslation, 1.0);
	fragColor = inColor;
	fragTexCoord = inTexCoord;
	fragTexOffsetScale = inTexOffsetScale;
}