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

void main() {
	gl_Position = proj * view * trans * v_position;
	gl_Position += vec4(0,0,0,0.0001);
	coords = v_texCoords;
	norm = normalize(v_normal);
	dist = gl_Position.w * 0.00125;
};

#shader fragment
#version 330 core

in vec2 coords;
in vec3 norm;
in float dist;

out vec4 color;

void main() {
	vec4 epic_blue =   vec4(0.0, 0.8, 1.0, 2.0 - (2 * dist));
	vec4 deep_purple = vec4(0.8, 0.0, 0.9, 2.0 - (2 * dist));
	color = mix(epic_blue, deep_purple, dist*1.2);
};