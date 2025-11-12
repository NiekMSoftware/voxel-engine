#pragma once

class IMouse;
class IKeyboard;


class Input
{
public:
	Input(const IKeyboard* keyboard, const IMouse* mouse);
	~Input();

	const IKeyboard& GetKeyboard() const;
	const IMouse& GetMouse() const;

private:
	const IMouse *const pMouse;
	const IKeyboard *const pKeyboard;
};