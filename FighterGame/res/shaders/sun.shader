#shader vertex
#version 330 core

layout(location = 0) in vec4 v_position;

uniform mat4 proj;
uniform mat4 view;

uniform float time;

void main() {
	vec2 pos = v_position.xy;
	mat2 rot = mat2(
		vec2(cos(time), sin(time)), vec2(-sin(time), cos(time))
	);
	pos *= rot;
	gl_Position = proj * view * vec4(pos, 0.0f, 1.0f);
};

#shader fragment
#version 330 core

out vec4 color;

void main() {
	color = vec4(1.0f, 0.5f, 0.0f, 1.0f);
};