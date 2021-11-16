#version 330 core

in vec3 color;

out vec4 FragColor;

void main()
{
    vec2 coord = gl_PointCoord - vec2(0.5);
    if(length(coord) > 0.5) discard;
    FragColor = vec4(color, 1.0f);
}
