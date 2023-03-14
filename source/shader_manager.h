#pragma once

#include "../vendor/glad.h"

#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderManager
{
public:
	
	ShaderManager(const std::string& vertexShaderPath,const std::string& fragmentShaderPath);

public:
	// Get the shader program ID
	inline GLuint getProgramID() const { return m_ProgramID; }

	// Activate the shader
	void execute() const;

	// Uniform functions
	void setBoolean(const std::string& uniformTag, GLboolean value) const;

	void setInteger(const std::string& uniformTag, GLint     value) const;

	void setFloat  (const std::string& uniformTag, GLfloat   value) const;

private:

	void checkCompilationErrors(GLuint shader, std::string type);

private:
	// Shader program ID
	GLuint m_ProgramID;
};


