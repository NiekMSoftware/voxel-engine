#include "WindowsGraphics.h"
#include "framework/Game.h"
#include <glad/glad.h>
#include <iostream>

WindowsGraphics::WindowsGraphics() 
{
	glfwInit();
	glfwWindowHint(GL_DEPTH_BUFFER_BIT, 16);
	glfwWindowHint(GL_DEPTH_BITS, 16);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

	pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PC Based OpenGL ES", nullptr, nullptr);
	if (!pWindow) 
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(pWindow);
	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initialize GLAD!\n";
		glfwDestroyWindow(pWindow);
		glfwTerminate();
	}

	std::cout << "Successfully initialized the Windows Graphics target!\n";
}

void WindowsGraphics::Quit() 
{
	glfwDestroyWindow(pWindow);
	glfwTerminate();
}

void WindowsGraphics::SwapBuffer() 
{
	glFlush();
	glfwSwapBuffers(pWindow);
	glfwPollEvents();
}

GLFWwindow &WindowsGraphics::Window() const 
{
	return *pWindow;
}