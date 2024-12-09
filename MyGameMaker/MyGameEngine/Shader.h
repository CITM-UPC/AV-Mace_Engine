#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

using namespace std;

struct ShaderProgramSource {
	string VertexSource;
	string FragmentSource;
};


class Shader {
private:
	string m_FilePath;
	unsigned int m_RendererID;
	unordered_map<string, int> m_UniformLocationCache;

public:
	Shader() = default;
	Shader(const string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	string GetFilePath() const { return m_FilePath; }

	//Set uniforms
	void SetUniform4f(const string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const string& name, int value);
	void SetUniformMat4f(const string& name, const glm::mat4& matrix);
	void SetUniform1iv(const string& name, int size, int value[]);
	void SetUniformBool(const string& name, bool value);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);

	int GetUniformLocation(const string& name);
};
