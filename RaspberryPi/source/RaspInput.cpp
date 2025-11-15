// Global includes
#include "RaspInput.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <linux/input.h>

// framework includes
#include "framework/input/IInput.h"

// == RASP MOUSE IMPL
// 

RaspMouse::RaspMouse(Display& display, Window& window) : display(display), window(window) {}

bool RaspMouse::GetButtonDown(const MouseButton button) const
{
	int root_x, root_y;
	int win_x, win_y;
	uint mask_return;
	Window window_returned;
	XQueryPointer(&display, window, &window_returned, &window_returned, &root_x, &root_y, &win_x, &win_y, &mask_return);

	switch (button)
	{
	case MouseButton::LEFT:		return mask_return & Button1MotionMask;
	case MouseButton::RIGHT:	return mask_return & Button3MotionMask;
	case MouseButton::MIDDLE:	return mask_return & Button2MotionMask;
	default:					return false;
	}
}

glm::vec2 RaspMouse::GetPosition() const
{
	int root_x, root_y;
	int win_x, win_y;
	uint mask_return;
	Window window_returned;
	XQueryPointer(&display, window, &window_returned, &window_returned, &root_x, &root_y, &win_x, &win_y, &mask_return);

	return glm::vec2{ win_x, win_y };
}

float RaspMouse::GetScrollDelta() const
{
	return 0;
}

//
// == END OF RASP MOUSE IMPL

//X11 imput support (for VNC compatibility)
#include <X11/Xlib.h>
#include <X11/keysym.h>

// == RASP KEYBOARD IMPL
//

RaspKeyboard::RaspKeyboard() : keyDown{ new bool[256] {false} }
{
	display = XOpenDisplay(nullptr);
	if (display)
	{
		useX11 = true;
	}
	else
	{
		FindKeyboardLocation();
		pthread_create(&keyboardThread, nullptr, &ProcessKeyboardThread, this);
	}
}

bool RaspKeyboard::GetKey(Key key) const
{
	// Query X11 keyboard state
	char keys[32];
	XQueryKeymap(display, keys);

	// Check which X11 key is pressed
	auto isPressed = [&](const KeySym sym) -> bool
		{
			const KeyCode kc = XKeysymToKeycode(display, sym);
			return (keys[kc / 8] & (1 << (kc % 8))) != 0;
		};

	// Set the keyDown and return it
	switch (key)
	{
	case Key::A:	return keyDown[KEY_A] = isPressed(XK_a);
	case Key::B:	return keyDown[KEY_B] = isPressed(XK_b);
	case Key::C:	return keyDown[KEY_C] = isPressed(XK_c);
	case Key::D:	return keyDown[KEY_D] = isPressed(XK_d);
	case Key::E:	return keyDown[KEY_E] = isPressed(XK_e);
	case Key::F:	return keyDown[KEY_F] = isPressed(XK_f);
	case Key::G:	return keyDown[KEY_G] = isPressed(XK_g);
	case Key::H:	return keyDown[KEY_H] = isPressed(XK_h);
	case Key::I:	return keyDown[KEY_I] = isPressed(XK_i);
	case Key::J:	return keyDown[KEY_J] = isPressed(XK_j);
	case Key::K:	return keyDown[KEY_K] = isPressed(XK_k);
	case Key::L:	return keyDown[KEY_L] = isPressed(XK_l);
	case Key::M:	return keyDown[KEY_M] = isPressed(XK_m);
	case Key::N:	return keyDown[KEY_N] = isPressed(XK_n);
	case Key::O:	return keyDown[KEY_O] = isPressed(XK_o);
	case Key::P:	return keyDown[KEY_P] = isPressed(XK_p);
	case Key::Q:	return keyDown[KEY_Q] = isPressed(XK_q);
	case Key::R:	return keyDown[KEY_R] = isPressed(XK_r);
	case Key::S:	return keyDown[KEY_S] = isPressed(XK_s);
	case Key::T:	return keyDown[KEY_T] = isPressed(XK_t);
	case Key::U:	return keyDown[KEY_U] = isPressed(XK_u);
	case Key::V:	return keyDown[KEY_V] = isPressed(XK_v);
	case Key::W:	return keyDown[KEY_W] = isPressed(XK_w);
	case Key::X:	return keyDown[KEY_X] = isPressed(XK_x);
	case Key::Y:	return keyDown[KEY_Y] = isPressed(XK_y);
	case Key::Z:	return keyDown[KEY_Z] = isPressed(XK_z);

	case Key::NUM_0:	return keyDown[KEY_0] = isPressed(XK_0);
	case Key::NUM_1:	return keyDown[KEY_1] = isPressed(XK_1);
	case Key::NUM_2:	return keyDown[KEY_2] = isPressed(XK_2);
	case Key::NUM_3:	return keyDown[KEY_3] = isPressed(XK_3);
	case Key::NUM_4:	return keyDown[KEY_4] = isPressed(XK_4);
	case Key::NUM_5:	return keyDown[KEY_5] = isPressed(XK_5);
	case Key::NUM_6:	return keyDown[KEY_6] = isPressed(XK_6);
	case Key::NUM_7:	return keyDown[KEY_7] = isPressed(XK_7);
	case Key::NUM_8:	return keyDown[KEY_8] = isPressed(XK_8);
	case Key::NUM_9:	return keyDown[KEY_9] = isPressed(XK_9);

	case Key::TAB:			return keyDown[KEY_TAB] = isPressed(XK_Tab);
	case Key::CAPS_LOCK:	return keyDown[KEY_CAPSLOCK] = isPressed(XK_Caps_Lock);
	case Key::SHIFT_LEFT:	return keyDown[KEY_LEFTSHIFT] = isPressed(XK_Shift_L);
	case Key::CTRL_LEFT:	return keyDown[KEY_LEFTCTRL] = isPressed(XK_Control_L);
	case Key::ALT_LEFT:		return keyDown[KEY_LEFTALT] = isPressed(XK_Alt_L);
	case Key::ESCAPE:		return keyDown[KEY_ESC] = isPressed(XK_Escape);
	case Key::RIGHT_SHIFT:	return keyDown[KEY_RIGHTSHIFT] = isPressed(XK_Shift_R);
	case Key::ENTER:		return keyDown[KEY_ENTER] = isPressed(XK_Return);
	case Key::ARROW_UP:		return keyDown[KEY_UP] = isPressed(XK_Up);
	case Key::ARROW_RIGHT:	return keyDown[KEY_RIGHT] = isPressed(XK_Right);
	case Key::ARROW_DOWN:	return keyDown[KEY_DOWN] = isPressed(XK_Down);
	case Key::ARROW_LEFT:	return keyDown[KEY_LEFT] = isPressed(XK_Left);
	case Key::SPACE:		return keyDown[KEY_SPACE] = isPressed(XK_space);

	case Key::F1:   return keyDown[KEY_F1] = isPressed(XK_F1);
	case Key::F2:   return keyDown[KEY_F2] = isPressed(XK_F2);
	case Key::F3:   return keyDown[KEY_F3] = isPressed(XK_F3);
	case Key::F4:   return keyDown[KEY_F4] = isPressed(XK_F4);
	case Key::F5:   return keyDown[KEY_F5] = isPressed(XK_F5);
	case Key::F6:   return keyDown[KEY_F6] = isPressed(XK_F6);
	case Key::F7:   return keyDown[KEY_F7] = isPressed(XK_F7);
	case Key::F8:   return keyDown[KEY_F8] = isPressed(XK_F8);
	case Key::F9:   return keyDown[KEY_F9] = isPressed(XK_F9);
	case Key::F10:  return keyDown[KEY_F10] = isPressed(XK_F10);
	case Key::F11:  return keyDown[KEY_F11] = isPressed(XK_F11);
	case Key::F12:  return keyDown[KEY_F12] = isPressed(XK_F12);

	default:
		std::cout << "ERROR::INPUT::Rasp Keycode not supported: " << static_cast<int>(key) << std::endl;
		return false;
	}
}

void RaspKeyboard::FindKeyboardLocation()
{
	const std::string ev{ FindActiveKeyboardEv() };
	keyboardLocation = "/dev/input/" + ev;
}

std::string RaspKeyboard::FindActiveKeyboardEv() const
{
	std::ifstream devicesFile;
	devicesFile.open("/proc/bus/input/devices");

	std::stringstream devicesStream;

	devicesStream << devicesFile.rdbuf();
	devicesFile.close();

	std::string devices = devicesStream.str();

	// The index in the file for our current new line position.
	std::size_t newLinePos{ 0 };

	// The EV value for the current device.
	std::string ev{};

	// Moves every new line in devices file.
	do
	{
		// Checks for the Handlers field.
		// Always caches it, so we can use it if we find the correct device.
		if (devices.substr(newLinePos + 1, 11) == "H: Handlers")
		{
			// Index of the end of the line.
			std::size_t nextNewLine{ devices.find('\n', newLinePos + 1) - 1 };

			// The contents of the EV line.
			std::string line{ devices.substr(newLinePos + 1, nextNewLine - (newLinePos + 1)) };

			// The index of the last space, so we can get the last value.
			std::size_t lastSpace{ line.rfind(' ') };

			// The very last value, which is the event.
			ev = line.substr(lastSpace + 1, line.length() - lastSpace + 1);
		}

		// Checks if current line start with the EV field.
		if (devices.substr(newLinePos + 1, 5) == "B: EV")
		{
			// The start of the value, while skipping the 'B: EV'.
			std::size_t start{ newLinePos + 7 };

			newLinePos = devices.find('\n', newLinePos + 1);

			// Grabs the string value of the EV field.
			std::string stringValue{ devices.substr(start, newLinePos - start) };

			// Moves the hex string into an uint.
			unsigned int value;
			std::stringstream ss;
			ss << std::hex << stringValue;
			ss >> value;

			// Defines the mask for a keyboard.
			constexpr unsigned int keyboardMask{ 0x120013 };

			if ((value & keyboardMask) == keyboardMask)
				break;

			continue;
		}


		newLinePos = devices.find('\n', newLinePos + 1);
	} while (newLinePos != std::string::npos);

	return ev;
}

void* RaspKeyboard::ProcessKeyboardThread(void* arg)
{
	const RaspKeyboard* input = static_cast<RaspKeyboard*>(arg);
	FILE* fKeyboard = fopen(((RaspKeyboard*)arg)->keyboardLocation.c_str(), "r");

	if (fKeyboard == nullptr)
	{
		printf("Keyboard file not found.");
		pthread_exit(nullptr);
	}

	input_event event;

	while (true)
	{
		fread(&event, sizeof(input_event), 1, fKeyboard);

		if (event.type == (__u16)EV_KEY)
			input->keyDown[event.code] = event.value > 0;
	}
}

//
// == END OF RASP KEYBOARD IMPL

// EoF

