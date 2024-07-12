#version 330 core

in vec3 vertexColor;
in vec3 normal_camera;
in vec3 lightDirection_camera;

out vec3 color;

void main() {
    vec3 unit_normal = normalize(normal_camera);
    vec3 unit_lightDirection = normalize(lightDirection_camera);
    float cos_theta = clamp(dot(unit_normal, unit_lightDirection),0,1);

    color = vertexColor * cos_theta;
}
