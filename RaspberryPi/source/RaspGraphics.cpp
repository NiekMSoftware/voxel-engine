#include "RaspGraphics.h"
#include <EGL/egl.h>
#include "EGLState.h"
#include "RaspMouse.h"
#include "XWindow.h"

RaspGraphics::RaspGraphics() : window(new XWindow())
{
	window->CreateWindow();
}

void RaspGraphics::Quit()
{
	XDestroyWindow(&window->GetDisplay(), window->GetWindow());
}

void RaspGraphics::SwapBuffer()
{
	EGLState state = window->GetState();
	eglSwapBuffers(state.display, state.surface);
}

XWindow& RaspGraphics::Window() const
{
	return *window;
}
