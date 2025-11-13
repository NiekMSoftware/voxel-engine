#include "Game.h"
#include "input/Input.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Interfaces
#include "IGraphics.h"
#include "input/IInput.h"

// Voxel Engine
#include "voxel_engine/world/Chunk.h"

#ifdef WINDOWS_BUILD
// #include "WindowsInput.h"
#endif

Game::Game(std::unique_ptr<const Input> input, std::unique_ptr<IGraphics> graphics)
	: pInput(std::move(input)), pGraphics(std::move(graphics)), deltaTime(0) {

	pRenderer = std::make_unique<VoxelRenderer>();
	pRenderer->Initialize();

#if WINDOWS_BUILD
	pRenderer->SetRenderDistance(6);
#endif

#if Raspberry_BUILD
	pRenderer->SetRenderDistance(4);
#endif

	pChunkMgr = std::make_unique<ChunkManager>(pRenderer->GetCubeVBO());

	// Initialize camera
	pCamera = std::make_unique<Camera>(glm::vec3(8.0f, 35.f, 8.0f));
	pCamera->SetMovementSpeed(15.0f);

	// Generate 128×128×32 world (8×8×2 chunks)
	for (int x = 0; x < 8; x++) {
		for (int z = 0; z < 8; z++) {
			for (int y = 0; y < 2; y++) {  // 2 vertical chunk layers
				Chunk *chunk = pChunkMgr->CreateChunk(glm::ivec3(x, y, z));

				// Fill chunks based on height
				for (int i = 0; i < CHUNK_SIZE; i++) {
					for (int j = 0; j < CHUNK_SIZE; j++) {
						for (int k = 0; k < CHUNK_SIZE; k++) {
							// Calculate world height
							int worldY = y * CHUNK_SIZE + j;

							if (worldY >= 32) continue;  // Above terrain

							Voxel voxel;
							if (worldY == 31) {
								voxel.type = 1; voxel.textureID = 0;  // Grass top
							} else if (worldY >= 28) {
								voxel.type = 2; voxel.textureID = 1;  // Dirt
							} else {
								voxel.type = 3; voxel.textureID = 2;  // Stone
							}
							chunk->SetVoxel(i, j, k, voxel);
						}
					}
				}
			}
		}
	}
}

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

		UpdateCamera();

		// Set up the viewport
		ClearScreen();
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glm::mat4 view = pCamera->GetViewMatrix();
		glm::mat4 proj = pCamera->GetProjectionMatrix(ASPECT_RATIO);

		pRenderer->Render(pChunkMgr->GetAllChunks(), view, proj);

		printf("average frames: %f\n", averageFPS);

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
}

void Game::UpdateCamera() {
	const Input &input = GetInput();
	const IKeyboard &keyboard = input.GetKeyboard();
	const IMouse &mouse = input.GetMouse();

	// Movement
	if (keyboard.GetKey(Key::W)) pCamera->MoveForward(deltaTime);
	if (keyboard.GetKey(Key::S)) pCamera->MoveBackward(deltaTime);
	if (keyboard.GetKey(Key::A)) pCamera->MoveLeft(deltaTime);
	if (keyboard.GetKey(Key::D)) pCamera->MoveRight(deltaTime);
	if (keyboard.GetKey(Key::SPACE)) pCamera->MoveUp(deltaTime);
	if (keyboard.GetKey(Key::CTRL_LEFT)) pCamera->MoveDown(deltaTime);

	// Speed control
	if (keyboard.GetKey(Key::SHIFT_LEFT)) {
		pCamera->SetMovementSpeed(5.6f);  // Sprint
	} else {
		pCamera->SetMovementSpeed(4.3f);  // Normal
	}

	// Camera rotation - Arrow Keys
	const float rotationSpeed = 500.0f;
	if (keyboard.GetKey(Key::ARROW_LEFT)) {
		pCamera->ProcessMouseMovement(-rotationSpeed * deltaTime, 0.0f);
	}
	if (keyboard.GetKey(Key::ARROW_RIGHT)) {
		pCamera->ProcessMouseMovement(rotationSpeed * deltaTime, 0.0f);
	}
	if (keyboard.GetKey(Key::ARROW_UP)) {
		pCamera->ProcessMouseMovement(0.0f, rotationSpeed * deltaTime);
	}
	if (keyboard.GetKey(Key::ARROW_DOWN)) {
		pCamera->ProcessMouseMovement(0.0f, -rotationSpeed * deltaTime);
	}

	const glm::vec2 mousePos = mouse.GetPosition();
	pCamera->ProcessMousePosition(mousePos.x, mousePos.y);
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