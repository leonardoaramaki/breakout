#include "particle_system.hpp"

panda2d::ParticleSystem::ParticleSystem(
    const uint numParticles,
    const glm::vec2 particleSize,
    const glm::vec2 sourcePosition,
    const bool loop
) 
    : m_Loop(loop)
{
    LOG("ParticleSystem constructor");
    m_Particles.reserve(numParticles);
    static std::random_device random;
    static std::uniform_real_distribution<float> dist(-0.6f, 0.6f);
    static auto color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    for (int i=0; i<numParticles; i++) {
        m_Particles.push_back({ 
            .Position=sourcePosition,
            .PositionStart=sourcePosition,
            .Velocity=glm::vec2(dist(random), dist(random)),
            .ColorStart=color,
            .ColorEnd=color,
            .Size=particleSize,
            .SizeStart=particleSize,
            .SizeEnd=glm::vec2(0.1f, 0.1f),
            .LifeRemaining=1.0f,
            .Lifetime=1.0f,
            .Active=true
        });
    }
}

panda2d::ParticleSystem::~ParticleSystem() {
    LOG("ParticleSystem destructed");
}

void panda2d::ParticleSystem::ResetParticle(Particle& particle)
{
    static std::random_device random;
    static std::uniform_real_distribution<float> velDist(-0.6f, 0.6f);
    static auto color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    particle.Position=particle.PositionStart;
    particle.Size=particle.SizeStart;
    particle.SizeEnd=glm::vec2(0.0f, 0.0f);
    particle.Velocity=glm::vec2(velDist(random), velDist(random));
    particle.ColorStart=color;
    particle.LifeRemaining=1.0f;
    particle.Lifetime=1.0f;
    particle.Active=m_Loop;
}

void panda2d::ParticleSystem::Update()
{
    static std::random_device random;
    static std::uniform_real_distribution<float> lifetimeDist(0.005f, 0.007f);
    for (auto& particle : m_Particles) {
        if (m_Loop && particle.LifeRemaining <= 0.0f) {
            ResetParticle(particle);
        }
        auto span=lifetimeDist(random);
        particle.Position.x += particle.Velocity.x;
        particle.Position.y += particle.Velocity.y;
        particle.LifeRemaining -= span;
        particle.Active = (particle.LifeRemaining > 0.0f);
    }
}

const std::vector<panda2d::Particle>& panda2d::ParticleSystem::GetParticles() const
{
    return m_Particles;
}

