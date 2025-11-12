//Set up the Raspberry/Linux systems

// Global includes
#include "XWindow.h"
#include "RaspGraphics.h"
#include "RaspInput.h"

// framework includes
#include "framework/Game.h"
#include "framework/input/Input.h"
#include "framework/input/IInput.h"

RaspGraphics* graphics;
Input* input;
Game* game;

int main()
{
    // Create graphics and input
    graphics = new RaspGraphics();
    input = new Input(new RaspKeyboard(), new RaspMouse(graphics->Window().GetDisplay(), graphics->Window().GetWindow()));

    // Start game with (input, graphics) abstracted from [Pi4 / Windows] platforms
    game = new Game(input, graphics);
    game->Start();
    delete game;
    return 0;
}
