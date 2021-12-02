#shader vertex
#version 330 core

layout(location = 0) in vec4 vertex;

uniform vec4 in_color;

out vec4 col;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    col = in_color;
}

#shader fragment
#version 330 core

in vec4 col;
out vec4 color;

void main()
{
    color = col;
}