#include "thirdparty/stb_image.h"
#include "renderer.hpp"

panda2d::Renderer::Renderer(const float width, const float height)
    : m_ShaderProgram{nullptr}
    , m_ViewPortWidth{width}
    , m_ViewPortHeight{height}
    , m_Scene{panda2d::Scene()}
{
    LOG("Renderer constructor called");
    // configure stbi
    stbi_set_flip_vertically_on_load(true);
    // compile shaders
    auto shaders = std::vector<Shader>();
    shaders.reserve(2);
    
    shaders.emplace_back("src/shaders/particles.vector.glsl", GL_VERTEX_SHADER);
    shaders.emplace_back("src/shaders/particles.fragment.glsl", GL_FRAGMENT_SHADER);
    m_ParticleShader.AddShaders(std::move(shaders));
    shaders.emplace_back("src/shaders/vector.glsl", GL_VERTEX_SHADER);
    shaders.emplace_back("src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    m_RootShader.AddShaders(std::move(shaders));

    m_ShaderProgram = &m_RootShader;

    // set viewport
    glViewport(0, 0, width, height);
    glUseProgram(m_ShaderProgram->GetRendererID());
}

void panda2d::Renderer::Clear(panda2d::Vec4&& backgroundColor)
{
    // clear screen
    glClearColor(
        backgroundColor.r,
        backgroundColor.g,
        backgroundColor.b,
        backgroundColor.a
    );
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // init matrices
    glm::mat4 projectionMatrix = glm::ortho(0.0f, m_ViewPortWidth, m_ViewPortHeight, 0.0f, 0.1f, 100.0f);
    GLint projectionLocation = glGetUniformLocation(m_ShaderProgram->GetRendererID(), "u_projection");
    if (projectionLocation >= 0) {
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
    } else {
        LOGE("u_projection location could not be found for program ");
        exit(EXIT_FAILURE);
    }
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    GLint viewLocation = glGetUniformLocation(m_ShaderProgram->GetRendererID(), "u_view");
    if (viewLocation >= 0) {
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(viewMatrix));
    } else {
        LOGE("u_view location could not be found");
        exit(EXIT_FAILURE);
    }
}

void panda2d::Renderer::DrawQuad(
    const Vec2& pos, 
    const Vec2& size,
    const Vec4& color,
    const std::string& texPath
) {
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 projectionMatrix = glm::ortho(0.0f, m_ViewPortWidth, m_ViewPortHeight, 0.0f, 0.1f, 100.0f);
    GLint projectionLocation = glGetUniformLocation(m_ShaderProgram->GetRendererID(), "u_projection");
    if (projectionLocation >= 0) {
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
    } else {
        LOGE("u_projection location could not be found for program ");
        exit(EXIT_FAILURE);
    }
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    GLint viewLocation = glGetUniformLocation(m_ShaderProgram->GetRendererID(), "u_view");
    if (viewLocation >= 0) {
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(viewMatrix));
    } else {
        LOGE("u_view location could not be found");
        exit(EXIT_FAILURE);
    }
    // vertices
    std::vector<glm::vec3> vertices{
        glm::vec3 {
            pos.x, pos.y + size.h, -1.0f, // bottom-left
        },
        glm::vec3 {
            pos.x + size.w, pos.y + size.h, -1.0f, // bottom-right
        },
        glm::vec3 {
            pos.x + size.w, pos.y, -1.0f, // top-right
        },
        glm::vec3 {
            pos.x, pos.y, -1.0f // top-left
        }
    };

    // init opengl buffers
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(glm::vec3) * vertices.size(),
        vertices.data(),
        GL_STATIC_DRAW);

    GLuint indexBufferObject;
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
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

    // check if we have a texture, otherwise use a solid color
    const bool solidColor = texPath.empty();
    GLuint colorBuffer = 0;
    if (solidColor) {
        auto vertexColors = new float[]{
            color.r, color.g, color.b, color.a,
            color.r, color.g, color.b, color.a,
            color.r, color.g, color.b, color.a,
            color.r, color.g, color.b, color.a
        };
        glGenBuffers(1, &colorBuffer);
        glEnablei(GL_BLEND, colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                sizeof(GL_FLOAT)*16,
                vertexColors,
                GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                4,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*)0);
        delete[] vertexColors;
    } else {
        // Texture
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
            (void*)0); // pointer to first component
        int widthImg, heightImg, numColCh;
        unsigned char* bytes;
        bytes = stbi_load(texPath.data(), 
            &widthImg,
            &heightImg,
            &numColCh,
            0);
        GLuint texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
        stbi_image_free(bytes);

        GLint tex0Uni = glGetUniformLocation(m_ShaderProgram->GetRendererID(), "tex0");
        if (tex0Uni >= 0) {
            glUniform1i(tex0Uni, 0);
        } else {
            LOGE("tex0 location could not be found");
            exit(EXIT_FAILURE);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Bind
    GLint modelMatrixLocation = glGetUniformLocation(m_ShaderProgram->GetRendererID(), "u_model");
    if (modelMatrixLocation >= 0) {
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
    } else {
        LOGE("u_model location could not be found");
        exit(EXIT_FAILURE);
    }
    // Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Unbind
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &indexBufferObject);
    if (colorBuffer > 0) {
        glDeleteBuffers(1, &colorBuffer);
    }
}

void panda2d::Renderer::DrawQuad(
        Vec2&& pos, 
        Vec2&& size, 
        Vec4&& color,
        std::string&& texPath
){
    DrawQuad(pos, size, color, texPath);
}

void panda2d::Renderer::DrawTriangle(Vec2 pos, const size_t width, const size_t height)
{
}

void panda2d::Renderer::Draw(const Sprite& sprite) const
{
    auto shaderProgram = m_ShaderProgram->GetRendererID();
    auto model = sprite.Model();
	auto texture = sprite.Texture();
    auto vao = sprite.VAO();
    // Bind
    glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texture);
    GLint modelMatrixLocation = glGetUniformLocation(shaderProgram, "u_model");
    if (modelMatrixLocation >= 0) {
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, model);
    } else {
        LOGE("u_model location could not be found");
        exit(EXIT_FAILURE);
    }
    GLint tex0Uni = glGetUniformLocation(shaderProgram, "tex0");
    if (tex0Uni >= 0) {
        glUniform1i(tex0Uni, 0);
    } else {
        LOGE("tex0 location could not be found");
        exit(EXIT_FAILURE);
    }
    // Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Unbind
    glBindVertexArray(0);
}

bool panda2d::Renderer::Collides(
        const panda2d::Sprite& s1, const panda2d::Sprite& s2) const
{
    return (s1.boundRect.x < s2.boundRect.x + s2.boundRect.width && s1.boundRect.x + s1.boundRect.width > s2.boundRect.x &&
            s1.boundRect.y < s2.boundRect.y + s2.boundRect.height && s1.boundRect.y + s1.boundRect.height > s2.boundRect.y);
}

GLuint panda2d::Renderer::GetShaderProgramId() const {
    return m_ShaderProgram->GetRendererID();
}

void panda2d::Renderer::AddParticles(const ParticleSystem& ps) {
    m_Scene.AddParticles(ps);
}

void panda2d::Renderer::DrawParticles()
{
    m_ShaderProgram = &m_ParticleShader;
    glUseProgram(m_ParticleShader.GetRendererID());
    auto& pArray = m_Scene.GetParticles();
    for (auto it=pArray.begin(); it != pArray.end(); it++) {
        // remove particle system if all of its particles get inactive
        bool inactiveParticleSystem = true;
        for (const auto& p : it->GetParticles()) {
            if (p.Active) {
                inactiveParticleSystem=false;
                glm::vec2 size = glm::lerp(p.SizeEnd,p.SizeStart,p.LifeRemaining);
                auto alpha = glm::lerp(0.0f,1.0f,p.LifeRemaining);
                DrawQuad(
                    {p.Position.x, p.Position.y},
                    {size.x,size.y},
                    {p.ColorStart.r,p.ColorStart.g,p.ColorStart.b,alpha},
                    ""); 
            }
        }
        if (inactiveParticleSystem) {
            pArray.erase(it--);
        } else {
            it->Update();
        }
    }
    m_ShaderProgram = &m_RootShader;
    glUseProgram(m_RootShader.GetRendererID());
}

