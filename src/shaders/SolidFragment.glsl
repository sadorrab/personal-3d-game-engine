#version 410 core

in float lightIntensity;
in vec4 fragmentColor;
out vec4 color;

void main() {
    vec4 baseColor = fragmentColor;
    color = (lightIntensity + 0.5) * baseColor;
}
