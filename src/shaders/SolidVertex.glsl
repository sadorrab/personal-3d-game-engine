#version 410 core

layout(location = 0) in vec3 vertexPosition_model;
layout(location = 1) in vec3 vertexNormal_model;
layout(location = 2) in vec4 vertexColor;
uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJECTION;
out float lightIntensity;
out vec4 fragmentColor;

void main() {
    gl_Position = PROJECTION * VIEW * MODEL * vec4(vertexPosition_model, 1.0);

    vec3 lightPosition_global = vec3(0, 0, 0); 
    vec3 lightDirection = lightPosition_global - (VIEW * MODEL * vec4(vertexNormal_model, 1)).xyz;
    lightIntensity = clamp(dot(normalize(vertexNormal_model), normalize(lightDirection)), 0, 1);

    fragmentColor = vertexColor;
}
