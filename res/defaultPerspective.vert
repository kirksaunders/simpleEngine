#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 modelCFrame;
uniform mat4 modelRotation;
uniform mat4 cameraInverse;
uniform mat4 projection;

uniform vec4 modelSize;

out vec2 texCoords;

out vec3 faceNormal;
out vec3 vertexPosition;

void main() {
    vertexPosition = (modelCFrame * (vec4(position, 1.0f) * modelSize)).xyz;
    faceNormal = normalize((modelRotation * (vec4(normal.xyz, 1.0f) * modelSize)).xyz);
    texCoords = texCoord;

    gl_Position = projection * cameraInverse * vec4(vertexPosition.xyz, 1.0f);
}