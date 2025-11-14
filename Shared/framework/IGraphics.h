#pragma once

/** @brief A simple interface class; its basically just simple access to graphics. */
class IGraphics
{
public:
	virtual ~IGraphics() = default;

	virtual void SwapBuffer() = 0;
	virtual void Quit() = 0;

	virtual void BeginImGuiFrame() = 0;
	virtual void EndImGuiFrame() = 0;
};