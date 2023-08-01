#include "window.hpp"

#define WINDOW_X 400
#define WINDOW_Y 400

Window::Window()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOGE("Failed to init sdl video subsystem!");
        exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_Window = SDL_CreateWindow(
        "Breakout", WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (m_Window == nullptr) {
        LOGE("Failed to create window!");
        exit(EXIT_FAILURE);
    }

    m_Context = SDL_GL_CreateContext(m_Window);
    if (m_Context == nullptr) {
        LOGE("Failed create opengl context!");
        exit(EXIT_FAILURE);
    }
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        LOGE("Failed to load glad");
        exit(EXIT_FAILURE);
    }
    LOG("Window constructed!");
}

Window::~Window()
{
    SDL_GL_DeleteContext(m_Context);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
    LOG("Window destroyed!");
}

void Window::MainLoop()
{
    auto renderer = panda2d::Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);

    // paddle specification
    const auto PADDLE_SPEED = 15.0f;
    const auto PADDLE_WIDTH = 60.0f;
    const auto PADDLE_HEIGHT = 20.0f;
    const auto sx = WINDOW_WIDTH / 2.0f - PADDLE_WIDTH / 2.0f;
    const auto sy = WINDOW_HEIGHT - 2*PADDLE_HEIGHT;
    panda2d::Sprite paddle(
        { .x=sx, .y=sy }, // position 
        PADDLE_WIDTH, // width
        PADDLE_HEIGHT, // height
        { .r=0x20, .g=0x40, .b=0x43, .a=0xFF} // color
    );

	paddle.SetTexture("resources/texture.png");

    // ball specification
    const auto BALL_SPEED = 30.0f;
    panda2d::Sprite ball(
        { .x=WINDOW_WIDTH/2.0f, .y=WINDOW_HEIGHT/2.0f }, // position 
        10, // width
        10, // height
        { .r=0xFF, .g=0x00, .b=0x00, .a=0xFF} // color
    );

    // bricks specification
    const auto COLS = 10;
    const auto ROWS = 8;
    const auto BRICK_HEIGHT = 10.0f;
    const auto BRICK_WIDTH = 50.0f;
    const auto BRICK_PADDING = 4;
    std::vector<panda2d::Sprite> bricks;
    bricks.reserve(COLS * ROWS);
    std::array<bool, COLS*ROWS> destroyed;
    destroyed.fill(false);

    for (auto r = 0; r < ROWS; r++) {
        panda2d::Color color{ .a=0xff };
        switch(r) {
            case 0:
            case 1: {
                color.r=0xff;
            } break; 
            case 2:
            case 3: {
                color.r=0xff;
                color.g=0x7f;
            } break; 
            case 4:
            case 5: {
                color.g=0xff;
            } break; 
            case 6:
            case 7: {
                color.r=0xff;
                color.g=0xff;
            } break; 
        }
        for (auto c = 0; c < COLS; c++) {
            auto& brick = bricks.emplace_back(
                panda2d::Vec2{ 
                        .x=(WINDOW_WIDTH / COLS / 3.0f) + c * (WINDOW_WIDTH / COLS),
                        .y=BRICK_HEIGHT * r + BRICK_PADDING * r 
                    }, 
                    BRICK_WIDTH, 
                    BRICK_HEIGHT,
                    std::move(color)
            );
       }
    }

    // Define game borders
    panda2d::Sprite top(
        panda2d::Vec2{ .x=0.0f, .y=0.0f },
        WINDOW_WIDTH-1.0f, 1.0f,
        panda2d::Color{}
    );
    panda2d::Sprite bottom(
        panda2d::Vec2{ .x=0.0f, .y=WINDOW_HEIGHT-1.0f },
        WINDOW_WIDTH, 1.0f,
        panda2d::Color{}
    );
    panda2d::Sprite left(
        panda2d::Vec2{ .x=0.0f, .y=0.0f },
        1.0f, WINDOW_HEIGHT-1.0f,
        panda2d::Color{}
    );
    panda2d::Sprite right(
        panda2d::Vec2{ .x=WINDOW_WIDTH-1.0f, .y=0.0f },
        1.0f, WINDOW_HEIGHT-1.0f,
        panda2d::Color{}
    );


    // define background sprite
    panda2d::Sprite bg(
        panda2d::Vec2{.x=0.0f, .y=0.0f},
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        panda2d::Color{.r=0x00,.g=0x00,.b=0xff,.a=0xff}
    );
    bg.SetTexture("resources/starfield2.png");

    auto direction = panda2d::Vec2{ .x=0.0f, .y=0.1f };

    // Main Loop
    bool quit = false;
    bool waitingForInput = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            auto state = SDL_GetKeyboardState(nullptr);
            // perspective is orthogonal and starts from the top
            //      (0, 0) +-------+ (width, 0)
            //             |       |
            //            ...     ...
            //             |       |
            // (0, height) +-------+ (width, height)
            if (state[SDL_SCANCODE_RIGHT]) {
                if (waitingForInput) {
                    waitingForInput=false;
                    direction.y=0.1;
                }
                if (!renderer.Collides(paddle, right)) {
                    paddle.Move(1.0f * PADDLE_SPEED, 0.0f);
                }
            } else if (state[SDL_SCANCODE_LEFT]) {
                if (waitingForInput) {
                    waitingForInput=false;
                    direction.y=0.1;
                }
                if (!renderer.Collides(paddle, left)) {
                    paddle.Move(-1.0f * PADDLE_SPEED, 0.0f);
                }
            } else if (state[SDL_SCANCODE_ESCAPE]) {
                quit=true;
            }
        }

        // Clear black 
        renderer.Clear({ .r=0.0f, .g=0.0f, .b=0.0f, .a=1.0f });
        renderer.Draw(bg);

        for (int i=0; i<bricks.size(); i++) {
            if (destroyed[i])
                continue;
            panda2d::Sprite* brick = &bricks[i];
            // Check collision with brick
            if (renderer.Collides(*brick, ball)) {
                // destroy brick, change direction
                direction.y = -direction.y;
                destroyed[i] = true;
                renderer.AddParticles(panda2d::ParticleSystem(100, // number of particles
   glm::vec2(15.0f, 15.0f), // particle size
   glm::vec2(brick->boundRect.x, brick->boundRect.y), // particle system position
   false)); // loop
                continue;
            }
            if (!destroyed[i])
                renderer.Draw(*brick);
        }

        ball.Move(direction.x, direction.y * BALL_SPEED);

        // Draw paddle
        renderer.Draw(paddle);
        // Draw ball
        renderer.Draw(ball);

        // Check collisions
        if (renderer.Collides(ball, paddle)) {
            // Change direction  
            auto x1 = paddle.boundRect.x;
            auto x2 = ball.boundRect.x;
            auto w1 = paddle.boundRect.width;
            auto w2 = ball.boundRect.width;
            auto normalized = 0.1 + (x2 - x1) / (w1 - w2) * 0.6f;
            normalized = (normalized < 0.3f) ? -8 * normalized : 4 * normalized;
            direction.y = -direction.y;
            direction.x = normalized;
        } else if (renderer.Collides(ball, top)) {
            // bounce
            direction.y = -direction.y;
        } else if (renderer.Collides(ball, bottom)) {
            // reset ball state
            direction.x = 0.0f;
            direction.y = 0.0f;
            ball.MoveTo(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f);
            renderer.Draw(ball);
            waitingForInput = true;
        } else if (renderer.Collides(ball, left)) {
            // bounce
            direction.x = -direction.x;
        } else if (renderer.Collides(ball, right)) {
            // bounce
            direction.x = -direction.x;
        }
        renderer.DrawParticles();

        // SDL_Delay(16);

        // swap buffer
        SDL_GL_SwapWindow(m_Window);
    }
}
