//Set up the Raspberry/Linux systems
#include "framework/input/Input.h"

#include <iostream>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include "include/XWindow.h"

#include <EGL/egl.h>

#include "framework/Game.h"
#include "framework/input/IInput.h"
#include "include/RaspGraphics.h"
#include "include/RaspKeyboard.h"
#include "include/RaspMouse.h"

float lastTime{};
float deltaTime{ 0.2f };

int currentFrame = 0;

RaspGraphics* graphics;
Input* input;
Game* game;
int main()
{
    graphics = new RaspGraphics();

    PFNGLDEBUGMESSAGECALLBACKKHRPROC peglDebugMessageControlKHR = reinterpret_cast<PFNGLDEBUGMESSAGECALLBACKKHRPROC>(eglGetProcAddress("glDebugMessageCallback"));
    if (!(peglDebugMessageControlKHR != 0)) {
        printf("failed to eglGetProcAddress eglDebugMessageControlKHR\n");
    }
    else {
        GLDEBUGPROCKHR DebugFn = [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
            {
                switch (severity)
                {
                case GL_DEBUG_SEVERITY_HIGH_KHR:
                case GL_DEBUG_SEVERITY_MEDIUM_KHR:
                    std::cout << message << std::endl;
                case GL_DEBUG_SEVERITY_LOW_KHR:
                case GL_DEBUG_SEVERITY_NOTIFICATION_KHR:
                default:
                    break; //Ignore.
                }
            };
        peglDebugMessageControlKHR(DebugFn, nullptr);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);
    }
    printf("This cross project was inspired by BUas Student Ferri de Lange\n");
    printf("This GPU supplied by  :%s\n", glGetString(GL_VENDOR));
    printf("This GPU supports GL  :%s\n", glGetString(GL_VERSION));
    printf("This GPU Renders with :%s\n", glGetString(GL_RENDERER));
    printf("This GPU Shaders are  :%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    input = new Input(new RaspKeyboard(), new RaspMouse(graphics->Window().GetDisplay(), graphics->Window().GetWindow()));

    // now we fire up our game giving it access to the input systems and graphics which are different on each platform but abstracted away   
    game = new Game(input, graphics);
    game->Start();

    return 0;
}
