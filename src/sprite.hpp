#pragma once

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>

#include "entity.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace panda2d {

class Sprite : public Entity {
public:
    Sprite(
            panda2d::Vec2&& position, 
            const size_t width, 
            const size_t height, 
            panda2d::Color&& color);
    Sprite(const Sprite& other) = default;
    Sprite(Sprite&& other);
    ~Sprite();
    void Move(const float dx, const float dy) override;
    void MoveTo(const float x, const float y);
    void SetTexture(std::string&& texPath);
    const GLfloat* Model() const;
    Sprite& operator=(const Sprite& other) = default;
    GLuint Texture() const;
    GLuint VAO() const;
    panda2d::Vec4 boundRect;

private:
    GLuint m_VertexArrayObject;
    GLuint m_VertexBuffer;
    GLuint m_IndexBuffer;
    GLuint m_Texture;
    std::vector<glm::vec3> m_Vertices;
    panda2d::Color m_Color;
};

} // namespace panda2d
 
