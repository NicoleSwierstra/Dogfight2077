#shader vertex
#version 330 core

layout(location = 0) in vec4 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 trans;

out vec2 coords;
out vec3 norm;

void main() {
	gl_Position = proj * view * trans * v_position;
	coords = v_texCoords;
	norm = v_normal;
};

#shader fragment
#version 330 core

in vec2 coords;
in vec3 norm;

out vec4 color;

void main() {
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};