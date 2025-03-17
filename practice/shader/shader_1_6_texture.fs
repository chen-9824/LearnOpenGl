#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D m_texture1;
uniform sampler2D m_texture2;

void main()
{
    FragColor = mix(texture(m_texture1, TexCoord), texture(m_texture2, TexCoord), 0.2);
};