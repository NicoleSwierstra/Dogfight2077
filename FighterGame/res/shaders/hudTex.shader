#shader vertex
#version 330 core

layout(location = 0) in vec4 v_position;
layout(location = 1) in vec2 v_texCoords;

out vec2 coords;

void main()
{
    gl_Position = vec4(v_position.xy, 0.0, 1.0);
    coords = v_texCoords;
}

#shader fragment
#version 330 core

uniform sampler2D tex;

in vec2 coords;
out vec4 color;

void main()
{
    color = texture(tex, coords);
}