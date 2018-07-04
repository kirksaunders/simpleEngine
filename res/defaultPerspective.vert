#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 modelCFrame;
uniform mat4 modelRotation;
uniform mat4 cameraInverse;
uniform mat4 projection;

uniform vec4 modelColor;
uniform vec4 lightPos;
uniform vec4 cameraPos;
uniform vec4 modelSize;

out vec2 texCoords;

out vec3 faceNormal;
out vec3 vertexPosition;
out vec3 lightPosition;
out vec3 cameraPosition;

void main() {
    vec4 vertexPos = modelCFrame * (vec4(position, 1.0f) * modelSize);
    gl_Position = projection * cameraInverse * vertexPos;

    vec4 worldNormal = vec4(normal, 1.0f) * modelSize;
    worldNormal = modelRotation * worldNormal;
    faceNormal = normalize(vec3(worldNormal));

    vertexPosition = vec3(vertexPos.xyz);
    lightPosition = vec3(lightPos.xyz);
    cameraPosition = vec3(cameraPos.xyz);
    texCoords = texCoord;
}