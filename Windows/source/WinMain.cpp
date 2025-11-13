// WinMain.cpp - Main executable function and entry-point for the Window Application.

// Global includes
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>

// framework includes
#include "framework/Game.h"
#include "framework/input/Input.h"
#include "WindowsGraphics.h"
#include "WindowsInput.h"

#ifdef WINDOWS_BUILD
// in case of a laptop with poor on board GPU, lets make sure we switch on any power gpu's we have to get max compatability
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

WindowsGraphics* pGraphics;
Input* pInput;
Game* pGame;

int main() 
{
	pGraphics = new WindowsGraphics();
	glfwSwapInterval(1);

	pInput = new Input(new WindowsKeyboard(pGraphics->Window()), new WindowsMouse(pGraphics->Window()));
	pGame = new Game(pInput, pGraphics);

	pGame->Start();
	delete pGame;
	return 0;
}