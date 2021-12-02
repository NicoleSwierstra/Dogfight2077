#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 trans;

out vec2 coords;

void main() {
	gl_Position = proj * view * position;
    coords = texCoords;
};

#shader fragment
#version 330 core

uniform sampler2D tex;

in vec2 coords;
out vec4 color;

void main() {
	color = texture(tex, coords);
};