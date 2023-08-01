#pragma once

#include <glad/glad.h>
#include <log.hpp>

#include <fstream>
#include <sstream>
#include <string>

namespace panda2d {
class Shader {
public:
    /*
    @brief Creates a shader instance. Reads a shader source file, create a shader object out of it and compile.
    @param shaderFilename Path to the shader source code.
    @param shaderType Type of shader object to create.
    */
    Shader(const std::string& shaderFilename, GLenum shaderType);
    Shader(Shader&& other);
    ~Shader();
    std::string source() const;
    GLuint ShaderObject() const;

private:
    std::string m_Filename;
    std::string m_Source;
    GLenum m_ShaderType;
    GLuint m_ShaderObject;
    void Compile();
};
} // namespace panda2d

