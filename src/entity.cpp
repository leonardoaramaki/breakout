#include "entity.hpp"

panda2d::Entity::Entity()
    : m_Model { translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) }
{
    LOG("Entity constuctor");
}

panda2d::Entity::~Entity()
{
    LOG("Entity destructor");
}

