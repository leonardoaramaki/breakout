#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "log.hpp"
#include "types.hpp"
#include "sprite.hpp"
#include "shader_program.hpp"
#include "particle_system.hpp"
#include "scene.hpp"

namespace panda2d {

class Renderer {
public:
    Renderer(const float width, const float height);
    void Clear(Vec4&& backgroundColor);
    void DrawTriangle(Vec2 pos, const size_t width, const size_t height);
    void DrawQuad(Vec2&& pos, Vec2&& size, Vec4&& color, std::string&& texPath);
    void DrawQuad(const Vec2& pos, const Vec2& size, const Vec4& color, const std::string& texPath);
    void Draw(const Sprite& sprite) const;
    void DrawParticles();
    void AddParticles(const ParticleSystem& ps);
    bool Collides(const Sprite& s1, const Sprite& s2) const;
    GLuint GetShaderProgramId() const;

private:
    float m_ViewPortWidth;
    float m_ViewPortHeight;
    // Cache all quads in the scene
    std::map<Vec2, GLuint> m_Quads;
    // Currently in use shader program
    ShaderProgram*  m_ShaderProgram;
    // Shaders used for all rendering except particles
    ShaderProgram m_RootShader;
    // Shaders used for particles only
    ShaderProgram m_ParticleShader;
    // Current scene
    Scene m_Scene;
};

} // namespace panda2d
