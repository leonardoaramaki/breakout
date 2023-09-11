#version 330 core

in vec2 v_texCoords;
uniform sampler2D tex0;

out vec4 color;

void main()
{
    color = texture(tex0, v_texCoords);
}
