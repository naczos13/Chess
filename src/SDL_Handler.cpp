#include "SDL_Handler.h"
#include <stdio.h>
#include <iostream>

SDL_Handler::SDL_Handler() {
	m_window = NULL;

	bool quit = false;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		cleanUp();
		return;
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
}


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

void SDL_Handler::loadPiecesTextures()
{
	textures[{Team::BLACK, PieceType::PAWN}]	= loadImage("../res/Chess_pdt60.png");
	textures[{Team::WHITE, PieceType::PAWN}]	= loadImage("../res/Chess_plt60.png");
	textures[{Team::BLACK, PieceType::KING}]	= loadImage("../res/Chess_kdt60.png");
	textures[{Team::WHITE, PieceType::KING}]	= loadImage("../res/Chess_klt60.png");
	textures[{Team::BLACK, PieceType::KNIGHT}]	= loadImage("../res/Chess_ndt60.png");
	textures[{Team::WHITE, PieceType::KNIGHT}]	= loadImage("../res/Chess_nlt60.png");
	textures[{Team::BLACK, PieceType::BISHOP}]	= loadImage("../res/Chess_bdt60.png");
	textures[{Team::WHITE, PieceType::BISHOP}]	= loadImage("../res/Chess_blt60.png");
	textures[{Team::BLACK, PieceType::QUEEN}]	= loadImage("../res/Chess_qdt60.png");
	textures[{Team::WHITE, PieceType::QUEEN}]	= loadImage("../res/Chess_qlt60.png");
	textures[{Team::BLACK, PieceType::ROOK}]	= loadImage("../res/Chess_rdt60.png");
	textures[{Team::WHITE, PieceType::ROOK}]	= loadImage("../res/Chess_rlt60.png");
}

void SDL_Handler::prepareBackgroundRender()
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

PieceType SDL_Handler::chooseNewPiece(Team turn)
{
	int y_draw = 0;
	Team team = Team::WHITE;
	Point optionTileSize{ SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 };

	if (turn == Team::BLACK)
	{
		y_draw = 3 * optionTileSize.y;
		team = Team::BLACK;
	}

	
	SDL_Rect rectangle = { 0,
						  y_draw,
						  optionTileSize.x,
						  optionTileSize.y };

	SDL_SetRenderDrawColor(m_renderer, 155, 103, 60, 255);
	SDL_RenderFillRect(m_renderer, &rectangle);
	SDL_Rect src = { 0, 0, CELL_WIDTH, CELL_HEIGHT };
	SDL_RenderCopy(m_renderer, textures.at({ team, PieceType::ROOK }), &src, &rectangle);


	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	rectangle.x = optionTileSize.x;
	SDL_RenderFillRect(m_renderer, &rectangle);
	SDL_RenderCopy(m_renderer, textures.at({ team, PieceType::KNIGHT }), &src, &rectangle);

	SDL_SetRenderDrawColor(m_renderer, 155, 103, 60, 255);
	rectangle.x = 2 * optionTileSize.x;
	SDL_RenderFillRect(m_renderer, &rectangle);
	SDL_RenderCopy(m_renderer, textures.at({ team, PieceType::BISHOP }), &src, &rectangle);

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	rectangle.x = 3 * optionTileSize.x;
	SDL_RenderFillRect(m_renderer, &rectangle);
	SDL_RenderCopy(m_renderer, textures.at({ team, PieceType::QUEEN }), &src, &rectangle);

	SDL_RenderPresent(m_renderer);
	SDL_UpdateWindowSurface(m_window);

	Point choosenPoint;

	while (SDL_WaitEvent(&m_event))
	{
		if (m_event.type == SDL_QUIT)
		{
			break;
		}

		if (m_event.type == SDL_MOUSEBUTTONDOWN)
		{
			choosenPoint.x = m_event.button.x / optionTileSize.x;
			choosenPoint.y = m_event.button.y / optionTileSize.y;

			if (choosenPoint.y >= y_draw / optionTileSize.y && choosenPoint.y < y_draw / optionTileSize.y + 1)
			{
				if (choosenPoint.x < 1)
				{
					return PieceType::ROOK;
				}
				else if (choosenPoint.x < 2)
				{
					return PieceType::KNIGHT;
				}
				else if (choosenPoint.x < 3)
				{
					return PieceType::BISHOP;
				}
				else if (choosenPoint.x <= 4)
				{
					return PieceType::QUEEN;
				}
			}
		}
	}

	return PieceType::EMPTY;

}

void SDL_Handler::prepareToRenderPieces(const std::vector<std::unique_ptr<Piece>>& pieces)
{
	for (const auto& piece : pieces)
	{
		if (!piece->isActive())
			continue;

		Point PiecePosition = piece->getPosition();
		SDL_Rect src = { 0, 0, CELL_WIDTH, 60 };
		SDL_Rect dest = { CELL_WIDTH * PiecePosition.x - 2,
						  CELL_HEIGHT * PiecePosition.y,
						  CELL_WIDTH,
						  CELL_HEIGHT };
		SDL_RenderCopy(m_renderer, piece->getTexture(), &src, &dest);
	}
}

void SDL_Handler::renderPossibleMoves(const std::vector<std::unique_ptr<Piece>>& pieces, const std::vector<PossibleMove>& possible)
{
	for (const auto& move : possible) {
		if ((move.XCoord % 2 == 0 && move.YCoord % 2 == 1) || (move.XCoord % 2 == 1 && move.YCoord % 2 == 0))
		{
			SDL_SetRenderDrawColor(m_renderer, 0, 134, 139, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(m_renderer, 164, 211, 238, 255);
		}
		SDL_Rect rectangle = { move.XCoord * CELL_WIDTH,
					  move.YCoord * CELL_HEIGHT,
					  CELL_WIDTH,
					  CELL_HEIGHT };
		SDL_RenderFillRect(m_renderer, &rectangle);
	}

	prepareToRenderPieces(pieces);
	SDL_RenderPresent(m_renderer);
	SDL_UpdateWindowSurface(m_window);
}

void SDL_Handler::refreshTheWindow(const std::vector<std::unique_ptr<Piece>>& pieces)
{
	prepareBackgroundRender();
	prepareToRenderPieces(pieces);

	SDL_RenderPresent(m_renderer);
	SDL_UpdateWindowSurface(m_window);
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
	// destroy all textures
	for (auto& [key, texture] : textures)
		SDL_DestroyTexture(texture);

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

SDL_Handler::~SDL_Handler()
{
	cleanUp();
}

