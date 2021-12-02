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
	norm = normalize(v_normal);
};

#shader fragment
#version 330 core

in vec2 coords;
in vec3 norm;

out vec4 color;

void main() {
	vec3 lightDir = normalize(vec3(0.0f, -10.0f, -4.0f));
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(vec3(0, 0, -1), reflectDir), 0.0), 64) * 0.4f;
	float lv = dot(-norm, lightDir) + spec;
	color = vec4(0.1 * lv, 0.1 * lv, 0.1 * lv, 1.0f);
};