#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoords;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_texCoords;

void main()
{
    v_texCoords = texCoords;
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
}
