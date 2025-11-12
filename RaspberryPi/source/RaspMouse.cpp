#include "RaspMouse.h"

RaspMouse::RaspMouse(Display& display, Window& window) :
	display(display),
	window(window)
{
}

bool RaspMouse::GetButtonDown(MouseButton button) const
{
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	uint mask_return;
	Window window_returned;
	XQueryPointer(
		&display,
		window,
		&window_returned,
		&window_returned,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);

	switch (button)
	{
	case MouseButton::LEFT:
	{
		return mask_return & Button1MotionMask;
	}
	case MouseButton::RIGHT:
	{
		return mask_return & Button3MotionMask;
	}
	case MouseButton::MIDDLE:
	{
		return mask_return & Button2MotionMask;
	}
	default:
	{
		return false;
	}
	}
}

glm::vec2 RaspMouse::GetPosition() const
{
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	uint mask_return;
	Window window_returned;
	XQueryPointer(
		&display,
		window,
		&window_returned,
		&window_returned,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);

	return glm::vec2{ win_x, win_y };
}

float RaspMouse::GetScrollDelta() const
{
	return 0;
}
