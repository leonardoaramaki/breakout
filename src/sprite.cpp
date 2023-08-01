#include "sprite.hpp"

panda2d::Sprite::Sprite(
        panda2d::Vec2&& position, 
        const size_t width, 
        const size_t height, 
        panda2d::Color&& color
)
    : m_VertexArrayObject{0}
    , m_VertexBuffer{0}
    , m_IndexBuffer{0}
    , m_Texture{0}
    , boundRect { .x=position.x, .y=position.y, .width=(float)width, .height=(float)height }
    , m_Color { std::move(color) }
{
    // Translates the model matrix to provided initial coordinates.
    m_Vertices = {
        glm::vec3 {
            position.x, position.y + height, -1.0f, // bottom-left
        },
        glm::vec3 {
            position.x + width, position.y + height, -1.0f, // bottom-right
        },
        glm::vec3 {
            position.x + width, position.y , -1.0f, // top-right
        },
        glm::vec3 {
            position.x, position.y, -1.0f // top-left
        }
    };
    glGenVertexArrays(1, &m_VertexArrayObject);
    glGenBuffers(1, &m_VertexBuffer);
    glGenBuffers(1, &m_IndexBuffer);
    glBindVertexArray(m_VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    // Vertices
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(glm::vec3) * m_Vertices.size(),
        m_Vertices.data(),
        GL_DYNAMIC_DRAW);

    const std::vector<GLuint> indexBufferData = { 0, 1, 2, 2, 3, 0 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indexBufferData.size() * sizeof(GLuint),
        indexBufferData.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, // index of the attribute
        3, // number of components per vertex attribute
        GL_FLOAT, // data type of each component
        GL_FALSE, // should be normalized
        0, // stride
        (void*)0); // pointer to first component
	// Generate texture based on color
    GLuint texBuffer;
    glGenBuffers(1, &texBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
    const std::vector<GLfloat> texData ={ 
        0.0f, 0.0f, // bottom-left
        1.0f, 0.0f, // bottom-right
        1.0f, 1.0f, // top-right 
        0.0f, 1.0f  // top-left
    };
    glBufferData(GL_ARRAY_BUFFER,
        texData.size() * sizeof(GL_FLOAT),
        texData.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, // index of the attribute
        2, // number of components per vertex attribute
        GL_FLOAT, // data type of each component
        GL_FALSE, // should be normalized
        0, // stride
        (void*)0); // pointer to  component

	unsigned char* bytes =
        new unsigned char[]{color.r,color.g,color.b,color.a};
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
	m_Texture = texture;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	delete[] bytes;
                
	glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

panda2d::Sprite::Sprite(Sprite&& other)
    : m_Vertices(std::move(other.m_Vertices))
    , m_VertexArrayObject(other.m_VertexArrayObject)
    , m_VertexBuffer(other.m_VertexBuffer)
    , m_IndexBuffer(other.m_IndexBuffer)
    , m_Texture(other.m_Texture)
    , m_Color(other.m_Color)
{
}

panda2d::Sprite::~Sprite()
{
    LOG("Sprite destructor");
    if (m_VertexArrayObject > 0) {
        glDeleteVertexArrays(1, &m_VertexArrayObject);
        glDeleteBuffers(1, &m_VertexBuffer);
        glDeleteBuffers(1, &m_IndexBuffer);
    }
}

void panda2d::Sprite::SetTexture(std::string&& texPath)
{
    m_Texture = panda2d::LoadTexture(std::move(texPath));
}

void panda2d::Sprite::Move(const float dx, const float dy)
{
    this->boundRect.x += dx;
    this->boundRect.y += dy;
	m_Model = glm::translate(m_Model, glm::vec3(dx, dy, 0.0f));
}

void panda2d::Sprite::MoveTo(const float x, const float y)
{
    this->boundRect.x = x;
    this->boundRect.y = y;
    auto width = this->boundRect.width;
    auto height = this->boundRect.height;
    m_Vertices = {
        glm::vec3 {
            x, y + height, -1.0f, // bottom-left
        },
        glm::vec3 {
            x + width, y + height, -1.0f, // bottom-right
        },
        glm::vec3 {
            x + width, y , -1.0f, // top-right
        },
        glm::vec3 {
            x, y, -1.0f // top-left
        }
    };
    m_Model = glm::mat4(1.0f);
}

const GLfloat* panda2d::Sprite::Model() const {
    return glm::value_ptr(m_Model);
}

GLuint panda2d::Sprite::Texture() const {
	return m_Texture;
}

GLuint panda2d::Sprite::VAO() const {
    return m_VertexArrayObject;
}


