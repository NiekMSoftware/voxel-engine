#include "Input.h"
#include "IInput.h"

Input::Input(std::unique_ptr<const IKeyboard> keyboard, std::unique_ptr<const IMouse> mouse)
	: pKeyboard(std::move(keyboard)), pMouse(std::move(mouse)) { }

const IKeyboard& Input::GetKeyboard() const
{
	return *pKeyboard;
}

const IMouse& Input::GetMouse() const
{
	return *pMouse;
}

// EoF