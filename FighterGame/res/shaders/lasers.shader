#shader vertex
#version 330 core

layout(location = 0) in vec4 v_position;
layout(location = 1) in float time;

uniform mat4 proj;
uniform mat4 view;

out float alpha;

void main() {
	gl_Position = proj * view * v_position;
	gl_Position += vec4(0, 0, 0, 0.0001);
	alpha = (1 - (gl_Position.w * 0.025)) * time;
};

#shader fragment
#version 330 core

in float alpha;

out vec4 color;

void main() {
	color = vec4(0.8f + alpha, -0.2f + alpha, -0.2f + alpha, alpha);
};