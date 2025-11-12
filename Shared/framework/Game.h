#pragma once
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
	const Input* const pInput;
	bool bQuitting{ false };
	float deltaTime;

	IGraphics* pGraphics;

private:
	int frameCount{ 0 };

public:
	Game(const Input* input, IGraphics* graphics);
	virtual ~Game();

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