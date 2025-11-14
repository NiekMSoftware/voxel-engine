#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include "framework/IGraphics.h"

// Custom deleter for GLFWwindow to ensure proper cleanup
struct GLFWwindowDeleter {
	void operator()(GLFWwindow *window) const {
		if (window)
		{
			glfwDestroyWindow(window);
		}
	}
};

class WindowsGraphics : public IGraphics 
{
public:
	WindowsGraphics();
	~WindowsGraphics() override;

	// Delete copy operations
	WindowsGraphics(const WindowsGraphics &) = delete;
	WindowsGraphics &operator=(const WindowsGraphics &) = delete;

	// Allow move operations
	WindowsGraphics(WindowsGraphics &&) = default;
	WindowsGraphics &operator=(WindowsGraphics &&) = default;

	void Quit() override;
	void SwapBuffer() override;

	void BeginImGuiFrame() override;
	void EndImGuiFrame() override;;

	GLFWwindow &Window() const;

private:
	std::unique_ptr<GLFWwindow, GLFWwindowDeleter> pWindow;
	bool bGLFWInitialized{ false };
};