#version 330 core
out vec4 FragColor;

in vec4 v_color;
in vec2 v_texcoord;

uniform sampler2D u_texture;

void main()
{
    FragColor = texture(u_texture, v_texcoord) * v_color;
} 
