#pragma once

#include <vector>
#include <random>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/compatibility.hpp>

#include "types.hpp"
#include "log.hpp"

namespace panda2d {

struct Particle {
    glm::vec2 Position;
    glm::vec2 PositionStart;
    glm::vec2 Velocity;
    glm::vec4 ColorStart;
    glm::vec4 ColorEnd;
    glm::vec2 Size;
    glm::vec2 SizeStart;
    glm::vec2 SizeEnd;
    float LifeRemaining;
    float Lifetime;
    bool Active;
};

class ParticleSystem {
public:
    ParticleSystem(const uint numParticles, const glm::vec2 particleSize, const glm::vec2 sourcePosition, const bool loop);
    virtual ~ParticleSystem();
    void Update();
    const std::vector<Particle>& GetParticles() const;

private:
    void ResetParticle(Particle& particle);
    std::vector<Particle> m_Particles;
    bool m_Loop;
};

} // namespace panda2d

