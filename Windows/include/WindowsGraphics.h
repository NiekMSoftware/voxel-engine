#pragma once

#include <GLFW/glfw3.h>
#include "framework/IGraphics.h"

class WindowsGraphics : public IGraphics 
{
public:
	WindowsGraphics();

	void Quit() override;
	void SwapBuffer() override;
	GLFWwindow &Window() const;

private:
	GLFWwindow* pWindow;
};