#pragma once
#include <memory>
#include <vector>

#include "voxel_engine/controllable/Camera.h"
#include "voxel_engine/renderer/VoxelRenderer.h"
#include "voxel_engine/world/ChunkManager.h"
using namespace voxel_engine;
using namespace voxel_engine::rendering;
using namespace voxel_engine::world;

#ifdef WINDOWS_BUILD
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif

class IGraphics;
class Input;

constexpr unsigned int WINDOW_WIDTH = 1024;
constexpr unsigned int WINDOW_HEIGHT = 768;
constexpr float ASPECT_RATIO = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

class Game
{
protected:
	std::unique_ptr<const Input> pInput;
	std::unique_ptr<IGraphics> pGraphics;

	std::unique_ptr<VoxelRenderer> pRenderer;
	std::unique_ptr<ChunkManager> pChunkMgr;
	std::unique_ptr<Camera> pCamera;

	bool bQuitting{ false };
	float deltaTime;

private:
	int frameCount{ 0 };

public:
	Game(std::unique_ptr<const Input> input, std::unique_ptr<IGraphics> graphics);
	virtual ~Game() = default;

	// Delete copy operations
	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	// Allow move operations
	Game(Game &&) = default;
	Game &operator=(Game &&) = default;

	void Start();
	void Quit();

	const Input& GetInput() const;

protected:
	void ProcessInput();
	void UpdateCamera();

	virtual void Update(float deltaTime);
	virtual void Render() {  }
	virtual void PostRender() {  }

private:
	void InitializeGLES();
	void ClearScreen();
};