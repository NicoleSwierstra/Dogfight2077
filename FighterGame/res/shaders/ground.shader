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
out float dist;
out float lv;

void main() {
	gl_Position = proj * view * trans * v_position;
	dist = gl_Position.w * 0.00125;
};

#shader fragment
#version 330 core

in vec2 coords;
in vec3 norm;
in float dist;
in flat float lv;

out vec4 color;

void main() {
	color = vec4(0.05, 0.05, 0.12, 2 - (2 * dist));
};