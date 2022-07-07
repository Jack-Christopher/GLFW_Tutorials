#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Shader
{
public:
    Shader(const char* shaderName);
    Shader(const char* vsSrc, const char* fsSrc);
    virtual ~Shader();

    void use() const;

    void setBool(const std::string& name, bool v0);
    void setInt(const std::string& name, int v0);
    void setFloat(const std::string& name, float v0);

    void setVec2(const std::string& name, float v0, float v1);
    void setVec3(const std::string& name, float v0, float v1, float v2);
    void setVec4(const std::string& name, float v0, float v1, float v2, float v3);

    void setVec2(const std::string& name, const glm::vec2& v);
    void setVec3(const std::string& name, const glm::vec3& v);
    void setVec4(const std::string& name, const glm::vec4& v);

    void setMat3(const std::string& name, const glm::mat3& m0);
    void setMat4(const std::string& name, const glm::mat4& m0);

private:
    GLuint createShaderProgram(const char* vsSrc, const char* fsSrc);

    GLuint m_program;
};

#endif // SHADER_HPP
