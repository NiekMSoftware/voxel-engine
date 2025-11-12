#pragma once
#include <string>

#include "framework/input/IInput.h"

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
	std::string FindActiveKeyboardEv();

	static void* ProcessKeyboardThread(void* arg);
};

