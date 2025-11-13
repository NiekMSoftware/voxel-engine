// Global includes
#include "XWindow.h"
#include "RaspGraphics.h"
#include "RaspInput.h"
#include <iostream>

// framework includes
#include "framework/Game.h"
#include "framework/input/Input.h"

RaspGraphics* graphics;
Input* input;
Game* game;

int main()
{
    try
    {
        // Create graphics first (owns the window)
        auto graphics = std::make_unique<RaspGraphics>();

        // Get reference to window for input creation
        XWindow& window = graphics->Window();

        // Create input system (references the window, doesn't own it)
        auto input = std::make_unique<Input>(
            std::make_unique<RaspKeyboard>(), 
            std::make_unique<RaspMouse>(window.GetDisplay(),window.GetWindow())
        );

        // Start game with (input, graphics) abstracted from [Pi4 / Windows] platforms
        auto game = std::make_unique<Game>(std::move(input), std::move(graphics));

        // Run the game
        game->Start();
        return 0;
    }
    catch (const std::exception& e)
	{
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
	}

}