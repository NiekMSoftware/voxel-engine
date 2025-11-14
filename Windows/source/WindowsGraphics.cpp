#include "WindowsGraphics.h"
#include "framework/Game.h"
#include <glad/glad.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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
	glfwSetInputMode(pWindow.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!\n";
		throw std::runtime_error("Failed to initialize GLAD");
	}

	// ImGUI initialisation
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	// Initialize platform/renderer backends
	ImGui_ImplGlfw_InitForOpenGL(pWindow.get(), true);
	ImGui_ImplOpenGL3_Init("#version 300 es");
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
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

void WindowsGraphics::BeginImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void WindowsGraphics::EndImGuiFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLFWwindow &WindowsGraphics::Window() const 
{
	return *pWindow;
}
