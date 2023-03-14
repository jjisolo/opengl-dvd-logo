#include "shader_manager.h"

ShaderManager::ShaderManager(const std::string& vertexShaderPath,const std::string& fragmentShaderPath)
{
	// Load shaders from the file
	std::string   vShaderSource;
	std::string   fShaderSource;

	std::ifstream vShaderFile(vertexShaderPath,   std::ios::in);
	std::ifstream fShaderFile(fragmentShaderPath, std::ios::in);

	if(!vShaderFile || !fShaderFile) {
		std::cerr << "Cannot open shader files!";
		return;
	}

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
       
        vShaderFile.close();
        fShaderFile.close();
       
        vShaderSource = vShaderStream.str();
        fShaderSource = fShaderStream.str();	

	}
	catch(std::ifstream::failure e)
    {
        std::cout << "Cannot read shader files" << e.what() << std::endl;
    }

    const char* vShaderCode = vShaderSource.c_str();
    const char* fShaderCode = fShaderSource.c_str();

    // Compile shaders


    // Compile vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompilationErrors(vertex, "VERTEX");

    // Compile fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompilationErrors(fragment, "FRAGMENT");

    // Create shader program
    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vertex);
    glAttachShader(m_ProgramID, fragment);
    glLinkProgram(m_ProgramID);
    checkCompilationErrors(m_ProgramID, "PROGRAM");
    
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void ShaderManager::checkCompilationErrors(GLuint shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
    	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}


void ShaderManager::execute() const
{
	glUseProgram(m_ProgramID);
}

void ShaderManager::setBoolean(const std::string& uniformTag, GLboolean value) const
{
	glUniform1i(glGetUniformLocation(m_ProgramID, uniformTag.c_str()), static_cast<int>(value));
}

void ShaderManager::setInteger(const std::string& uniformTag, GLint value) const
{
	glUniform1i(glGetUniformLocation(m_ProgramID, uniformTag.c_str()), value);
}

void ShaderManager::setFloat(const std::string& uniformTag, GLfloat value) const
{
	glUniform1f(glGetUniformLocation(m_ProgramID, uniformTag.c_str()), value);
}