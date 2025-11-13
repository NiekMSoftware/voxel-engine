#pragma once
#include "framework/IGraphics.h"

class XWindow;
struct EGLState;

class RaspGraphics : public IGraphics
{
public:
	RaspGraphics();

	void Quit() override;
	void SwapBuffer() override; 

	XWindow& Window() const;

private:
	XWindow* window;
	void SetupGLDebug() const;
};

