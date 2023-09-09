#include "SDL_Handler.h"
#include <stdio.h>
#include <iostream>


SDL_Texture* SDL_Handler::loadImage(std::string filename)
{
	SDL_Surface* loadedImage = NULL;

	loadedImage = IMG_Load(filename.c_str());

	if (loadedImage == NULL)
	{
		std::cout << "couldnt load " << filename << std::endl;
	}

	SDL_Texture* text = SDL_CreateTextureFromSurface(m_renderer, loadedImage);

	return text;
}

void SDL_Handler::renderBackground()
{
	bool white = true;

	// Iterate over the whole board
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			// Choose the color
			if (white)
			{
				// Choose white color as paint
				SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
			}
			else
			{
				// Choose gray color as paint
				SDL_SetRenderDrawColor(m_renderer, 155, 103, 60, 255);
			}
			white = !white;
			SDL_Rect rectangle = { i * SCREEN_WIDTH / 8,
								  j * SCREEN_HEIGHT / 8,
								  SCREEN_WIDTH / 8,
								  SCREEN_HEIGHT / 8 };
			// paint the specific rectangle
			SDL_RenderFillRect(m_renderer, &rectangle);
		}
		white = !white;
	}
}

void SDL_Handler::undoPieceRender(int x, int y)
{
	// Unrender the pice from the specific rectangle

	// Check the modulo to find out if the rectagle should be render Dark or light
	if ((x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1))
	{
		SDL_SetRenderDrawColor(m_renderer, LIGHT.r, LIGHT.g, LIGHT.b, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(m_renderer, DARK.r, DARK.g, DARK.b, 255);
	}
	SDL_Rect rectangle = { x * SCREEN_WIDTH / 8,
						  y * SCREEN_HEIGHT / 8,
						  SCREEN_WIDTH / 8,
						  SCREEN_HEIGHT / 8 };
	SDL_RenderFillRect(m_renderer, &rectangle);
}

void SDL_Handler::cleanUp()
{
	SDL_FreeSurface(m_screenSurface);
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
}


void SDL_Handler::DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text)
{
	if (text != nullptr)
	{
		SDL_RenderCopy(m_renderer, text, &source, &dest);
		SDL_RenderPresent(m_renderer);

		SDL_UpdateWindowSurface(m_window);
	}
	else
	{
		std::cout << "DrawRectangle: text was nullptr" << std::endl;
	}
}


SDL_Handler::SDL_Handler() {
	init();
}


SDL_Handler::~SDL_Handler()
{
	cleanUp();
}


bool SDL_Handler::init()
{
	m_window = NULL;
	m_screenSurface = NULL;

	bool quit = false;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		cleanUp();
		return false;
	}
	else
	{
		//Create window
		m_window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (m_window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		}
	}
	return true;
}
