#include <iostream>

#include "graphics/graphics.hpp"
#include "input/input.hpp"

#include "Midi.hpp"
#include "SongState.hpp"

#include "Visualizers/ColorBlocks.hpp"

int WindowWidth = 1200;
int WindowHeight = 700;

void windowResizeCB(float width, float height)
{
	WindowWidth = width;
	WindowHeight = height;
	// RecalculateDimensions();
}

window win(WindowWidth, WindowHeight, "Live Music Machine", &windowResizeCB);
inputManager inp(&win);

#define WIN_BACKGROUND_COLOR 34, 34, 34, 255

void initializeWindow(window& win)
{
	win.setBackgroundColor(WIN_BACKGROUND_COLOR);

	// shouldClose manages some state
	win.shouldClose();
	win.update();

	win.shouldClear(true);
	win.getBase()->setVerticalSyncEnabled(true);
	win.getBase()->setFramerateLimit(60);
}

int main()
{
	initializeWindow(win);

	SongState song;

	while (!win.shouldClose() && !inp.isPressed(inputCode::Space))
	{
		song.Update();

		Visualization_ColorBlocksUpdate(win, song);

		win.update();
	}
	return 0;
}
