#include <iostream>

#include "shader.hpp"

panda2d::Shader::Shader(const std::string& shaderFilename, GLenum shaderType)
    : m_Filename{shaderFilename}
    , m_ShaderObject{0}
    , m_ShaderType{shaderType}
{
    std::fstream fs;
    std::string line;
    std::stringstream out;

    fs.open(shaderFilename, std::ios::in);
    if (!fs.is_open()) {
        LOGE("Failed to open ", shaderFilename);
        exit(1);
    }

    while (std::getline(fs, line)) {
        out << line;
        out << "\n";
    }

    m_Source = out.str();

    fs.close();
    Compile();
    LOG("Shader ", m_ShaderObject, " constructed!");
}

panda2d::Shader::Shader(Shader&& other)
    : m_ShaderObject(other.m_ShaderObject)
    , m_ShaderType(other.m_ShaderType)
    , m_Source(std::move(other.m_Source))
{
    LOG("Shader move ctor called for ", m_ShaderObject);
}

void panda2d::Shader::Compile()
{
    m_ShaderObject = glCreateShader(m_ShaderType);
    const char* src = m_Source.c_str();
    glShaderSource(m_ShaderObject, 1, &src, nullptr);
    glCompileShader(m_ShaderObject);
    GLint status;
    glGetShaderiv(m_ShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLsizei length;
        GLchar message[1024];
        glGetShaderInfoLog(m_ShaderObject, 1024, &length, message);
        LOGE("Shader ", m_Filename, " didnt compile: ", message);
    }
}

GLuint panda2d::Shader::ShaderObject() const { return m_ShaderObject; }

panda2d::Shader::~Shader()
{
    LOG("Shader ", m_ShaderObject, " destructed");
}

std::string panda2d::Shader::source() const { return m_Source; }
