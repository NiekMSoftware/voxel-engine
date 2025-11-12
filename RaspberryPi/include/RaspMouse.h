#pragma once

#include <X11/Xlib.h>

#include "framework/input/IInput.h"

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
