#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <stdio.h>
#include <string>


struct color {
	int r, g, b;
};

class SDL_Handler
{
public:

	// Screen size
	static const int SCREEN_WIDTH = 480;
	static const int SCREEN_HEIGHT = 480;
	static const int CELL_WIDTH = SCREEN_WIDTH / 8;
	static const int CELL_HEIGHT = SCREEN_HEIGHT / 8;


	// constructor
	SDL_Handler();

	// window we'll be rendering to
	SDL_Window* m_window;

	// Surface contained by the window
	SDL_Surface* m_screenSurface;

	// Renderer
	SDL_Renderer* m_renderer;

	// Event (Mouseclick etc)
	SDL_Event m_event;
	
	// initialize the field
	// returns true, if everything worked
	bool init();

	// cleans up SDL stuff
	void cleanUp();

	// destructor
	~SDL_Handler();

	// renders the source rectangle of the texture to dest rectangle
	void DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text);

	// removes PieceRender
	void undoPieceRender(int x, int y);

	// gets filename, return texture of the jpg file
	SDL_Texture* loadImage(std::string filename);

	//renders the background of the field
	void renderBackground();

private:
	const color DARK{ 155, 103, 60 };
	const color LIGHT{ 255, 255, 255 };
};

