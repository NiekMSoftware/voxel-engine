#include "RaspKeyboard.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <linux/input.h>
#include "framework/input/IInput.h"

RaspKeyboard::RaspKeyboard() : keyDown{ new bool[256] {false} }
{
	FindKeyboardLocation();
	pthread_create(&keyboardThread, nullptr, &ProcessKeyboardThread, this);
}

bool RaspKeyboard::GetKey(Key key) const
{
	switch (key)
	{
	case Key::A: return keyDown[KEY_A];
	case Key::B: return keyDown[KEY_B];
	case Key::C: return keyDown[KEY_C];
	case Key::D: return keyDown[KEY_D];
	case Key::E: return keyDown[KEY_E];
	case Key::F: return keyDown[KEY_F];
	case Key::G: return keyDown[KEY_G];
	case Key::H: return keyDown[KEY_H];
	case Key::I: return keyDown[KEY_I];
	case Key::J: return keyDown[KEY_J];
	case Key::K: return keyDown[KEY_K];
	case Key::L: return keyDown[KEY_L];
	case Key::M: return keyDown[KEY_M];
	case Key::N: return keyDown[KEY_N];
	case Key::O: return keyDown[KEY_O];
	case Key::P: return keyDown[KEY_P];
	case Key::Q: return keyDown[KEY_Q];
	case Key::R: return keyDown[KEY_R];
	case Key::S: return keyDown[KEY_S];
	case Key::T: return keyDown[KEY_T];
	case Key::U: return keyDown[KEY_U];
	case Key::V: return keyDown[KEY_V];
	case Key::W: return keyDown[KEY_W];
	case Key::X: return keyDown[KEY_X];
	case Key::Y: return keyDown[KEY_Y];
	case Key::Z: return keyDown[KEY_Z];
	case Key::NUM_0: return keyDown[KEY_0];
	case Key::NUM_1: return keyDown[KEY_1];
	case Key::NUM_2: return keyDown[KEY_2];
	case Key::NUM_3: return keyDown[KEY_3];
	case Key::NUM_4: return keyDown[KEY_4];
	case Key::NUM_5: return keyDown[KEY_5];
	case Key::NUM_6: return keyDown[KEY_6];
	case Key::NUM_7: return keyDown[KEY_7];
	case Key::NUM_8: return keyDown[KEY_8];
	case Key::NUM_9: return keyDown[KEY_9];
	case Key::TAB: return keyDown[KEY_TAB];
	case Key::CAPS_LOCK: return keyDown[KEY_CAPSLOCK];
	case Key::SHIFT_LEFT: return keyDown[KEY_LEFTSHIFT];
	case Key::CTRL_LEFT: return keyDown[KEY_LEFTCTRL];
	case Key::ALT_LEFT: return keyDown[KEY_LEFTALT];
	case Key::ESCAPE: return keyDown[KEY_ESC] || keyDown[KEY_HOMEPAGE];
	case Key::RIGHT_SHIFT: return keyDown[KEY_RIGHTSHIFT];
	case Key::ENTER: return keyDown[KEY_ENTER];
	case Key::ARROW_UP: return keyDown[KEY_UP];
	case Key::ARROW_RIGHT: return keyDown[KEY_RIGHT];
	case Key::ARROW_DOWN: return keyDown[KEY_DOWN];
	case Key::ARROW_LEFT: return keyDown[KEY_LEFT];
	case Key::SPACE: return keyDown[KEY_SPACE];
	default: std::cout << "ERROR::INPUT::Rasp Keycode not supported: " << static_cast<int>(key) << std::endl; return false;
	}
}

void RaspKeyboard::FindKeyboardLocation()
{
	std::string ev{ FindActiveKeyboardEv() };
	keyboardLocation = "/dev/input/" + ev;
}

std::string RaspKeyboard::FindActiveKeyboardEv()
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
			{
				break;
			}

			continue;
		}


		newLinePos = devices.find('\n', newLinePos + 1);
	} while (newLinePos != std::string::npos);

	return ev;
}

void* RaspKeyboard::ProcessKeyboardThread(void* arg)
{
	RaspKeyboard* input = static_cast<RaspKeyboard*>(arg);

	FILE* fKeyboard;
	fKeyboard = fopen(((RaspKeyboard*)arg)->keyboardLocation.c_str(), "r");

	if (fKeyboard == nullptr)
	{
		printf("Keyboard file not found.");
		pthread_exit(nullptr);
		return nullptr;
	}

	input_event event;

	while (true)
	{
		fread(&event, sizeof(input_event), 1, fKeyboard);

		if (event.type == (__u16)EV_KEY)
		{
			input->keyDown[event.code] = event.value > 0;
		}
	}

	fclose(fKeyboard);
	pthread_exit(nullptr);
}

