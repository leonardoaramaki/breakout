#pragma once

#include <vector>

#include "log.hpp"
#include "shader.hpp"

namespace panda2d {

class ShaderProgram {
public:
    ShaderProgram();
    ShaderProgram(std::vector<Shader>&& shaders);
    ~ShaderProgram();
    void AddShaders(std::vector<Shader>&& shaders);
    GLuint GetRendererID() const;

 private:
    std::vector<Shader> mShaders;
    GLuint mProgramObject;
    void CompileAndLink();
};

} // namespace panda2d

