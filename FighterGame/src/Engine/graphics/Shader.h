#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string Vertex;
	std::string Geometry;
	std::string Fragment;
};

class Shader {
private:
	unsigned int m_ShaderId;
	unsigned int texture;
	std::unordered_map<std::string, unsigned int> uniform_cache;
public:
	Shader(const std::string& filepath);
	Shader(const std::string& vertex, const std::string& fragment);
	~Shader();

	void Bind() const;
	void UnBind() const;	

	void SetV4Uniforms(const std::string& name, float x, float y, float z, float w);
	void SetV3Uniforms(const std::string& name, float x, float y, float z);
	void SetV2Uniforms(const std::string& name, float x, float y);
	void SetV4Uniforms(const std::string& name, glm::vec4& v);
	void SetV3Uniforms(const std::string& name, glm::vec3& v);
	void SetV2Uniforms(const std::string& name, glm::vec2& v);

	void SetMat4Uniforms(const std::string& name, glm::mat4& matrix);
	void SetIntUniforms(const std::string& name, int i);
	void SetFloatUniforms(const std::string& name, float i);

	Shader* Duplicate();

private:
	unsigned int GetGLUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int compileShader(const std::string source, unsigned int type);
};