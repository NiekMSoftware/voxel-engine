#include "Game.h"
#include "input/Input.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "IGraphics.h"
#include "input/IInput.h"

#ifdef WINDOWS_BUILD
// #include "WindowsInput.h"
#endif

Game::Game(const Input* const input, IGraphics* const graphics)
	: pInput(input), deltaTime(0), pGraphics(graphics)
{
	// initialize stuff in here
}

Game::~Game() {
	delete pInput;
	delete pGraphics;
}

void Game::Start() {
	InitializeGLES();

	// Timing
	auto startTime = std::chrono::system_clock::now();
	auto lastTime = startTime;

	// uncomment to track average FPS:
	// float averageFPS{ 0 };

	while (!bQuitting) 
	{
		ProcessInput();
		auto time = std::chrono::system_clock::now();
		std::chrono::duration<float> delta = time - lastTime;

		deltaTime = delta.count();

		std::chrono::duration<float> elapsed = time - startTime;
		if (elapsed.count() > 0.25f && frameCount > 10) 
		{
			// averageFPS = (float)frameCount / elapsed.count();
			startTime = time;
			frameCount = 0;
		}

		// Set up the viewport
		ClearScreen();
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		Update(deltaTime);

		// Render your game here
		glFlush();
		pGraphics->SwapBuffer();
		lastTime = time;
		++frameCount;
	}

	pGraphics->Quit();
}

void Game::Quit()
{
	bQuitting = true;
}

const Input& Game::GetInput() const {
	return *pInput;
}

void Game::ProcessInput()
{
	const Input& input = GetInput();
	const IKeyboard& keyboard = input.GetKeyboard();
	const IMouse& mouse = input.GetMouse();

	// Leaving window
	if (keyboard.GetKey(Key::ESCAPE))
	{
		printf("\nQuitting Game..");
		Quit();
		return;
	}

	// Testing key outputs
	for (int i = (int)Key::A; i <= (int)Key::Z; ++i)
	{
		if (keyboard.GetKey((Key)i))
		{
			printf("%c\n", 'A' + i);
			break; 
		}
	}

	// Testing mouse outputs
	if (mouse.GetButtonDown(MouseButton::LEFT))		printf("Mouse-Left\n");
	if (mouse.GetButtonDown(MouseButton::MIDDLE))	printf("Mouse-Mid\n");
	if (mouse.GetButtonDown(MouseButton::RIGHT))	printf("Mouse-Right\n");
}

void Game::Update(float /*deltaTime*/)
{
	// Update your game logic here
}

void Game::InitializeGLES() {
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRangef(0.0f, 1.0f);
	glClearDepthf(1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glCullFace(GL_BACK);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Game::ClearScreen() {
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// EoF