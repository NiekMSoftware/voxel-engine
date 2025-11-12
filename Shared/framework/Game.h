#pragma once
#include <memory>
#include <vector>

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
constexpr unsigned int ASPECT_RATIO = WINDOW_WIDTH / WINDOW_HEIGHT;

class Game
{
protected:
	std::unique_ptr<const Input> pInput;
	std::unique_ptr<IGraphics> pGraphics;

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

	virtual void Update(float deltaTime);
	virtual void Render() {  }
	virtual void PostRender() {  }

private:
	void InitializeGLES();
	void ClearScreen();
};