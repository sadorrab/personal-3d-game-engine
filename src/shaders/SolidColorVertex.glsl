#version 330 core

layout(location = 0) in vec3 vertexPosition_local;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal_local;

out vec3 vertexColor;
out vec3 normal_camera;
out vec3 lightDirection_camera;

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJECTION;

void main() {
    gl_Position = PROJECTION * VIEW * MODEL * vec4(vertexPosition_local,1);

    vertexColor = vertexColor;

    vec3 vertexPosition_camera = (VIEW * MODEL * vec4(vertexPosition_local, 1)).xyz;
    vec3 viewDirection_camera = vec3(0,0,0) - vertexPosition_camera;

    vec3 lightPosition_world = (0,0,2);
    vec3 lightPosition_camera = (VIEW * vec4(lightPosition_world)).xyz;

    lightDirection_camera = lightPosition_camera + viewDirection_camera;
    normal_camera = (VIEW * MODEL * vec4(vertexNormal_local,0)).xyz
