#include "scene.hpp"

panda2d::Scene::Scene() {
    LOG("Scene constructed");
}

panda2d::Scene::~Scene() {
    LOG("Scene destructed");
}

const std::vector<panda2d::Sprite>& panda2d::Scene::GetSprites() const {
    return m_Sprites;
}

std::vector<panda2d::ParticleSystem>& panda2d::Scene::GetParticles() const {
    return const_cast<std::vector<panda2d::ParticleSystem>&>(m_Particles);
}

void panda2d::Scene::AddParticles(const panda2d::ParticleSystem ps) {
    m_Particles.push_back(std::move(ps));
}

void panda2d::Scene::AddSprite(const panda2d::Sprite sprite) {
    m_Sprites.push_back(std::move(sprite));
}

