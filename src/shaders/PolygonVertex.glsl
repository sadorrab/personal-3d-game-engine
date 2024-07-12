#version 330 core

layout(location = 0) in vec3 vertexPosition_local;
layout(location = 1) in vec3 vertexColor;
out vec3 fragmentColor;
uniform mat4 CAMERA;
uniform mat4 MODEL;

void main() {
    gl_Position =  CAMERA * MODEL * vec4(vertexPosition_local,1);
    fragmentColor = vertexColor;
}
