#include "Game.h"
#include "input/Input.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

// Interfaces
#include "IGraphics.h"
#include "input/IInput.h"

#ifdef WINDOWS_BUILD
#include "WindowsGraphics.h"
#endif

#ifdef WINDOWS_BUILD
// #include "WindowsInput.h"
#endif

Game::Game(std::unique_ptr<const Input> input, std::unique_ptr<IGraphics> graphics)
	: pInput(std::move(input)), pGraphics(std::move(graphics)), deltaTime(0) { }

void Game::Start() {
	InitializeGLES();

	// Timing
	auto startTime = std::chrono::system_clock::now();
	auto lastTime = startTime;

	// uncomment to track average FPS:
	float averageFPS{ 0 };

	while (!bQuitting) {
		ProcessInput();
		auto time = std::chrono::system_clock::now();
		std::chrono::duration<float> delta = time - lastTime;

		deltaTime = delta.count();

		std::chrono::duration<float> elapsed = time - startTime;
		if (elapsed.count() > 0.25f && frameCount > 10) {
			averageFPS = (float)frameCount / elapsed.count();
			startTime = time;
			frameCount = 0;
		}

		// Set up the viewport
		ClearScreen();
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		// Render ImGui
		pGraphics->BeginImGuiFrame();
		if (bShowDebugMode)
		{
			ImGui::Begin("Debug Info", &bShowDebugMode);
			ImGui::Text("FPS: %.1f", averageFPS);
			ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);
			ImGui::End();
		}

		pGraphics->EndImGuiFrame();

		glFlush();
		pGraphics->SwapBuffer();
		lastTime = time;
		++frameCount;
	}

	pGraphics->Quit();
}

void Game::Quit() {
	bQuitting = true;
}

const Input& Game::GetInput() const {
	return *pInput;
}

void Game::ProcessInput() {
	const Input& input = GetInput();
	const IKeyboard& keyboard = input.GetKeyboard();

	// Leaving window
	if (keyboard.GetKey(Key::ESCAPE)) {
		printf("\nQuitting Game..");
		Quit();
	}

	// Toggle Mouse
#ifdef WINDOWS_BUILD
	static bool lastAltState = false;
	const bool currentAltState = keyboard.GetKey(Key::ALT_LEFT);

	if (currentAltState && !lastAltState) {
		bMouseCaptured = !bMouseCaptured;

		if (const WindowsGraphics *winGraphics = dynamic_cast<WindowsGraphics*>(pGraphics.get())) 
		{
			glfwSetInputMode(&winGraphics->Window(), GLFW_CURSOR,
				bMouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}
	}
	lastAltState = currentAltState;
#endif

	static bool lastF3State = false;
	const bool currentF3State = keyboard.GetKey(Key::F3);
	if (currentF3State && !lastF3State) 
	{
		bShowDebugMode = !bShowDebugMode;
	}
	lastF3State = currentF3State;
}

void Game::Update(float /*deltaTime*/) {
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