#include "shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>

Shader::Shader(const char* shaderName)
{
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::stringstream vShaderStrStream;
    std::stringstream fShaderStrStream;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open("shaders/" + std::string(shaderName) + ".vs");
        fShaderFile.open("shaders/" + std::string(shaderName) + ".fs");
        vShaderStrStream << vShaderFile.rdbuf();
        fShaderStrStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
    }
    catch(const std::ifstream::failure& e)
    {
        std::cerr << "Error while loading shader:\n" << e.what() << std::endl;
    }

    m_program = createShaderProgram(vShaderStrStream.str().c_str(), fShaderStrStream.str().c_str());
}

Shader::Shader(const char* vsSrc, const char* fsSrc)
    : m_program {createShaderProgram(vsSrc, fsSrc)}
{
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::use() const
{
    glUseProgram(m_program);
}

void Shader::setBool(const std::string& name, bool v0)
{
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)v0);
}

void Shader::setInt(const std::string& name, int v0)
{
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), v0);
}

void Shader::setFloat(const std::string& name, float v0)
{
    glUniform1f(glGetUniformLocation(m_program, name.c_str()), v0);
}

void Shader::setVec2(const std::string& name, float v0, float v1)
{
    glUniform2f(glGetUniformLocation(m_program, name.c_str()), v0, v1);
}

void Shader::setVec3(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(glGetUniformLocation(m_program, name.c_str()), v0, v1, v2);
}

void Shader::setVec4(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(glGetUniformLocation(m_program, name.c_str()), v0, v1, v2, v3);
}

void Shader::setVec2(const std::string& name, const glm::vec2& v)
{
    glUniform2f(glGetUniformLocation(m_program, name.c_str()), v.x, v.y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& v)
{
    glUniform3f(glGetUniformLocation(m_program, name.c_str()), v.x, v.y, v.z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& v)
{
    glUniform4f(glGetUniformLocation(m_program, name.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::setMat3(const std::string& name, const glm::mat3& m0)
{
    glUniformMatrix3fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(m0));
}

void Shader::setMat4(const std::string& name, const glm::mat4& m0)
{
    glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(m0));
}

GLuint Shader::createShaderProgram(const char* vsSrc, const char* fsSrc)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsSrc, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
