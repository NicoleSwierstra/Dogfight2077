#shader vertex
#version 330 core

layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = vec4(vertex.x, -vertex.y, 0.0, 1.0);// *projection;
    TexCoords = vertex.zw;
}

#shader fragment
#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    color = vec4(textColor, texture(text, TexCoords).r);
}