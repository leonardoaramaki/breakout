#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <array>

#include <SDL2/SDL.h>
#include "entity.hpp"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "log.hpp"
#include "sprite.hpp"
#include "renderer.hpp"
#include "shader_program.hpp"
#include "types.hpp"
#include "particle_system.hpp"

class Window {
public:
    Window();
    ~Window();
    void MainLoop();

private:
    const size_t WINDOW_WIDTH = 800;
    const size_t WINDOW_HEIGHT = 600;
    SDL_Window* m_Window = nullptr;
    SDL_GLContext m_Context = nullptr;
};
