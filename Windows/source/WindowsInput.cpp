#pragma once
#include "WindowsInput.h"
#include "framework/input/IInput.h"
#include <GLFW/glfw3.h>
#include <iostream>

// == WINDOWS MOUSE IMPL
// 
WindowsMouse::WindowsMouse(GLFWwindow &window) : window(window) 
{
	glfwSetScrollCallback(&window, ScrollCallback);
}

bool WindowsMouse::GetButtonDown(MouseButton button) const {
	switch (button)
	{
	case MouseButton::LEFT: return glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_LEFT);
	case MouseButton::RIGHT: return glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_RIGHT);
	case MouseButton::MIDDLE: return glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_MIDDLE);
	default:std::cout << "ERROR::INPUT::WINDOWS Mouse button not supported: " << static_cast<int>(button) << std::endl; return false;
	}
}

glm::vec2 WindowsMouse::GetPosition() const {
	double xPosition, yPosition;
	glfwGetCursorPos(&window, &xPosition, &yPosition);
	return glm::vec2{ xPosition, yPosition };
}

float WindowsMouse::GetScrollDelta() const {
	return scrollDelta;
}

float WindowsMouse::scrollDelta{ 0.0f };

void WindowsMouse::ScrollCallback(GLFWwindow* /*window*/, double /*xOffset*/, double yOffset) {
	scrollDelta = static_cast<float>(yOffset);
}
//
// == END OF WINDOWS MOUSE IMPL

// == WINDOWS KEYBOARD IMPL
//
WindowsKeyboard::WindowsKeyboard(GLFWwindow &window) : window(window) { }

bool WindowsKeyboard::GetKey(Key key) const {
	switch (key) {
	case Key::A: return glfwGetKey(&window, GLFW_KEY_A);
	case Key::B: return glfwGetKey(&window, GLFW_KEY_B);
	case Key::C: return glfwGetKey(&window, GLFW_KEY_C);
	case Key::D: return glfwGetKey(&window, GLFW_KEY_D);
	case Key::E: return glfwGetKey(&window, GLFW_KEY_E);
	case Key::F: return glfwGetKey(&window, GLFW_KEY_F);
	case Key::G: return glfwGetKey(&window, GLFW_KEY_G);
	case Key::H: return glfwGetKey(&window, GLFW_KEY_H);
	case Key::I: return glfwGetKey(&window, GLFW_KEY_I);
	case Key::J: return glfwGetKey(&window, GLFW_KEY_J);
	case Key::K: return glfwGetKey(&window, GLFW_KEY_K);
	case Key::L: return glfwGetKey(&window, GLFW_KEY_L);
	case Key::M: return glfwGetKey(&window, GLFW_KEY_M);
	case Key::N: return glfwGetKey(&window, GLFW_KEY_N);
	case Key::O: return glfwGetKey(&window, GLFW_KEY_O);
	case Key::P: return glfwGetKey(&window, GLFW_KEY_P);
	case Key::Q: return glfwGetKey(&window, GLFW_KEY_Q);
	case Key::R: return glfwGetKey(&window, GLFW_KEY_R);
	case Key::S: return glfwGetKey(&window, GLFW_KEY_S);
	case Key::T: return glfwGetKey(&window, GLFW_KEY_T);
	case Key::U: return glfwGetKey(&window, GLFW_KEY_U);
	case Key::V: return glfwGetKey(&window, GLFW_KEY_V);
	case Key::W: return glfwGetKey(&window, GLFW_KEY_W);
	case Key::X: return glfwGetKey(&window, GLFW_KEY_X);
	case Key::Y: return glfwGetKey(&window, GLFW_KEY_Y);
	case Key::Z: return glfwGetKey(&window, GLFW_KEY_Z);
	case Key::NUM_0: return glfwGetKey(&window, GLFW_KEY_0);
	case Key::NUM_1: return glfwGetKey(&window, GLFW_KEY_1);
	case Key::NUM_2: return glfwGetKey(&window, GLFW_KEY_2);
	case Key::NUM_3: return glfwGetKey(&window, GLFW_KEY_3);
	case Key::NUM_4: return glfwGetKey(&window, GLFW_KEY_4);
	case Key::NUM_5: return glfwGetKey(&window, GLFW_KEY_5);
	case Key::NUM_6: return glfwGetKey(&window, GLFW_KEY_6);
	case Key::NUM_7: return glfwGetKey(&window, GLFW_KEY_7);
	case Key::NUM_8: return glfwGetKey(&window, GLFW_KEY_8);
	case Key::NUM_9: return glfwGetKey(&window, GLFW_KEY_9);
	case Key::TAB: return glfwGetKey(&window, GLFW_KEY_TAB);
	case Key::CAPS_LOCK: return glfwGetKey(&window, GLFW_KEY_CAPS_LOCK);
	case Key::SHIFT_LEFT: return glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT);
	case Key::CTRL_LEFT: return glfwGetKey(&window, GLFW_KEY_LEFT_CONTROL);
	case Key::ALT_LEFT:return glfwGetKey(&window, GLFW_KEY_LEFT_ALT);
	case Key::ESCAPE: return glfwGetKey(&window, GLFW_KEY_ESCAPE);
	case Key::RIGHT_SHIFT: return glfwGetKey(&window, GLFW_KEY_RIGHT_SHIFT);
	case Key::ENTER: return glfwGetKey(&window, GLFW_KEY_ENTER);
	case Key::ARROW_UP: return glfwGetKey(&window, GLFW_KEY_UP);
	case Key::ARROW_RIGHT: return glfwGetKey(&window, GLFW_KEY_RIGHT);
	case Key::ARROW_DOWN: return glfwGetKey(&window, GLFW_KEY_DOWN);
	case Key::ARROW_LEFT: return glfwGetKey(&window, GLFW_KEY_LEFT);
	case Key::SPACE: return glfwGetKey(&window, GLFW_KEY_SPACE);
	default: std::cout << "ERROR::INPUT::WINDOWS Keycode not supported: " << static_cast<int>(key) << std::endl; return false;
	}
}
//
// == END OF WINDOWS KEYBOARD IMPL

// EoF