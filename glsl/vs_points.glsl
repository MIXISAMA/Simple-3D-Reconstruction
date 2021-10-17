#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

out vec3 WorldPos;
out vec4 Color;

void main()
{
    Color = color;
    WorldPos = vec3(model * vec4(aPosition, 1.0));

    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
    gl_PointSize = gl_Position.z;
}
