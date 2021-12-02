#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 proj;
uniform mat4 trans;
uniform mat4 view;

void main() {
	gl_Position = (proj * view * trans * position) + vec4(0.0f, 0.0f, -0.0001f, 0.0f);
};

#shader fragment
#version 330 core
uniform float timer;

out vec4 color;

void main() {
	if (timer < -0.1f)
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	else if (timer > 0.05f)
		color = mix(vec4(0.3f, 0.7f, 1.0f, 1.0f), vec4(1.0f, 0.5f, 0.5f, 1.0f), timer);
	else
		color = vec4(0.0f, 0.5f, 1.0f, 1.0f);
};