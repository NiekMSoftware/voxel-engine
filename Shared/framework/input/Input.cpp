#include "Input.h"
#include "IInput.h"

Input::Input(const IKeyboard* const keyboard, const IMouse* const mouse)
	: pMouse(mouse), pKeyboard(keyboard)
{ }

Input::~Input()
{
	delete pMouse;
	delete pKeyboard;
}

const IKeyboard& Input::GetKeyboard() const
{
	return *pKeyboard;
}

const IMouse& Input::GetMouse() const
{
	return *pMouse;
}

// EoF