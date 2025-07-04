#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
	float time;
	float frameCount;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragWorldPos;
layout(location = 2) out vec3 fragWorldNormal;
layout(location = 3) out vec2 fragProjPos;
layout(location = 4) out float time;
layout(location = 5) out float frameCount;

void main() {
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

	fragColor = inColor.rgb;
	fragWorldPos = (ubo.model * vec4(inPosition, 1.0)).xyz;
	fragWorldNormal = (ubo.model * vec4(inNormal, 0.0)).xyz;
	fragProjPos = (ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0)).xy;
	time = ubo.time;
	frameCount = ubo.frameCount;
}