#include "WindowsGraphics.h"
#include "framework/Game.h"
#include <glad/glad.h>
#include <iostream>

WindowsGraphics::WindowsGraphics() 
{
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialize GLFW");
	}
	bGLFWInitialized = true;

	glfwWindowHint(GL_DEPTH_BUFFER_BIT, 16);
	glfwWindowHint(GL_DEPTH_BITS, 16);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

	GLFWwindow *rawWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
		"PC Based OpenGL ES", nullptr, nullptr);
	if (!rawWindow)
	{
		std::cout << "Failed to create GLFW window!\n";
		throw std::runtime_error("Failed to create GLFW window");
	}

	// Transfer ownership to unique_ptr
	pWindow.reset(rawWindow);

	glfwMakeContextCurrent(pWindow.get());
	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!\n";
		throw std::runtime_error("Failed to initialize GLAD");
	}
}

WindowsGraphics::~WindowsGraphics() 
{
	if (bGLFWInitialized) 
	{
		glfwTerminate();
	}
}

void WindowsGraphics::Quit() 
{
	pWindow.reset(); // Explicitly destroy window

	if (bGLFWInitialized)
	{
		glfwTerminate();
		bGLFWInitialized = false;
	}
}

void WindowsGraphics::SwapBuffer() 
{
	glFlush();
	glfwSwapBuffers(pWindow.get());
	glfwPollEvents();
}

GLFWwindow &WindowsGraphics::Window() const 
{
	return *pWindow;
}