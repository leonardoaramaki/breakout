#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "log.hpp"
#include "shader_program.hpp"

namespace panda2d {

class Entity {
public:
    Entity();
    virtual ~Entity();
    virtual void Move(const float dx, const float dy) = 0;

protected:
    glm::mat4 m_Model;
};

} // namespace panda2d

