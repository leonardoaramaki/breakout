#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 v_color;

void main()
{
    v_color = color;
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
}
