#include "XWindow.h"
#include "RaspGraphics.h"
#include <iostream>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include "EGLState.h"

RaspGraphics::RaspGraphics() : window(new XWindow())
{
    window->CreateWindow();
    SetupGLDebug();
}

void RaspGraphics::Quit()
{
    XDestroyWindow(&window->GetDisplay(), window->GetWindow());
}

void RaspGraphics::SwapBuffer()
{
    const EGLState state = window->GetState();
    eglSwapBuffers(state.display, state.surface);
}

XWindow& RaspGraphics::Window() const
{
    return *window;
}

void RaspGraphics::SetupGLDebug() const
{
    const PFNGLDEBUGMESSAGECALLBACKKHRPROC peglDebugMessageControlKHR = reinterpret_cast<PFNGLDEBUGMESSAGECALLBACKKHRPROC>(eglGetProcAddress("glDebugMessageCallback"));
    if (peglDebugMessageControlKHR == nullptr)
    {
        printf("failed to eglGetProcAddress eglDebugMessageControlKHR\n");
    }
    else
    {
        const GLDEBUGPROCKHR DebugFn = [](GLenum source, GLenum type, GLuint id,
            const GLenum severity, GLsizei length,
            const GLchar* message, const void* userParam)
            {
                switch (severity)
                {
                case GL_DEBUG_SEVERITY_HIGH_KHR:
                case GL_DEBUG_SEVERITY_MEDIUM_KHR:
                    std::cout << message << std::endl;
                    break;  
                case GL_DEBUG_SEVERITY_LOW_KHR:
                case GL_DEBUG_SEVERITY_NOTIFICATION_KHR:
                default:
                    break; 
                }
            };
        peglDebugMessageControlKHR(DebugFn, nullptr);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);
    }
}
