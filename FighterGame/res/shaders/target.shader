#shader vertex
#version 330 core

layout(location = 0) in vec4 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 trans;

out float h;
uniform float health;

void main() {
	gl_Position = proj * view * trans * v_position;
	h = health;
};

#shader fragment
#version 330 core

in float h;
out vec4 color;

void main() {
	color = mix(vec4(1, 0, 0, 1), vec4(1, 1, 1, 1), h);
};