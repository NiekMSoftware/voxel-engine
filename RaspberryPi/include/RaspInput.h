#pragma once
#include <string>
#include <X11/Xlib.h>

#include "framework/input/IInput.h"

// RaspMouse
class RaspMouse : public IMouse
{
public:
	RaspMouse(Display& display, Window& window);

	bool GetButtonDown(MouseButton button) const override;
	glm::vec2 GetPosition() const override;
	float GetScrollDelta() const override;

private:
	Display& display;
	Window& window;
};

// RaspKeyboard
class RaspKeyboard : public IKeyboard
{
public:
	RaspKeyboard();
	bool GetKey(Key key) const override;

private:
	pthread_t keyboardThread;
	bool* keyDown;
	std::string keyboardLocation;

	void FindKeyboardLocation();
	std::string FindActiveKeyboardEv() const;

	static void* ProcessKeyboardThread(void* arg);

	Display* display = nullptr;
	bool useX11 = false;
};


