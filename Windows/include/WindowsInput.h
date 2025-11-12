#pragma once

#include "framework/input/IInput.h"
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class WindowsKeyboard : public IKeyboard 
{
private:
	GLFWwindow &window;

public:
	WindowsKeyboard(GLFWwindow &window);
	bool GetKey(Key key) const override;
};

class WindowsMouse : public IMouse 
{
private:
	GLFWwindow &window;
	static float scrollDelta;

public:
	WindowsMouse(GLFWwindow &window);

	bool GetButtonDown(MouseButton button) const override;
	glm::vec2 GetPosition() const override;
	float GetScrollDelta() const override;

private:
	static void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
};