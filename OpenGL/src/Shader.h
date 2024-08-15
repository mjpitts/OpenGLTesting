#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <unordered_map>

#include "Renderer.h"

/* This struct allows us to return two items from our shader parsing function. */
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{

private:
	std::string m_Filepath;
	unsigned int m_RenderID;
	std::unordered_map<std::string, unsigned int> M_UniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);
	unsigned int Createshader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompilerShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
};