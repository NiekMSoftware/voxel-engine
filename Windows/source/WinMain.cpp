// WinMain.cpp - Main executable function and entry-point for the Window Application.

#undef RASPBERRY_PI

// Global includes
#include <iostream>
#include <memory>

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

int main() 
{
    try
    {
        // Create graphics first (owns the window)
        auto graphics = std::make_unique<WindowsGraphics>();
        glfwSwapInterval(1);

        // Get reference to window for input creation
        GLFWwindow &window = graphics->Window();

        // Create input system (references the window, doesn't own it)
        auto input = std::make_unique<Input>(
            std::make_unique<WindowsKeyboard>(window),
            std::make_unique<WindowsMouse>(window)
        );

        // Create game (takes ownership of input and graphics)
        auto game = std::make_unique<Game>(std::move(input), std::move(graphics));

        // Run the game
        game->Start();
    } 
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

	return 0;
}