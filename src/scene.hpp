#pragma once

#include <vector>

#include "particle_system.hpp"
#include "sprite.hpp"
#include "log.hpp"

namespace panda2d {

class Scene {
public:
    Scene();
    virtual ~Scene();
    void AddParticles(const ParticleSystem ps);
    void AddSprite(const Sprite sprite);
    const std::vector<Sprite>& GetSprites() const;
    std::vector<ParticleSystem>& GetParticles() const;
private:
    std::vector<Sprite> m_Sprites;
    std::vector<ParticleSystem> m_Particles;

};

} // namespace panda2d
   
