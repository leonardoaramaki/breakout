#include "shader_program.hpp"

panda2d::ShaderProgram::ShaderProgram(std::vector<Shader>&& shaders)
    : mShaders(std::move(shaders)) {
    CompileAndLink();
    LOG("ShaderProgram ", mProgramObject, " constructed!");
}

panda2d::ShaderProgram::ShaderProgram()
{
}

panda2d::ShaderProgram::~ShaderProgram() {
  glDeleteProgram(mProgramObject);
  LOG("ShaderProgram ", mProgramObject, " destructed!");
}

void panda2d::ShaderProgram::AddShaders(std::vector<Shader>&& shaders) {
    mShaders = std::move(shaders);
    CompileAndLink();
}

void panda2d::ShaderProgram::CompileAndLink() {
  mProgramObject = glCreateProgram();
  for (auto& shader : mShaders) {
    glAttachShader(mProgramObject, shader.ShaderObject());
  }
  glLinkProgram(mProgramObject);
  glValidateProgram(mProgramObject);
}

GLuint panda2d::ShaderProgram::GetRendererID() const { return mProgramObject; }

