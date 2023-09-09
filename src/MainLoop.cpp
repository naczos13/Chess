#include <SDL.h>
#include <stdio.h>
#include "MainLoop.h"
#include "SDL_Handler.h"
#include "Game.h"
#include <memory>


void MainLoop::run()
{
	SDL_Handler handler = SDL_Handler();
	handler.init();

	handler.renderBackground();

	std::unique_ptr<Game> game = std::make_unique<Game>(&handler);
	bool quit = false;

	int xStart = -1;
	int yStart = -1;
	int xEnd = -1;
	int yEnd = -1;
	Piece* clickedOn = nullptr;

	while (!quit)
	{
		while (SDL_WaitEvent(&handler.m_event))
		{
			switch (handler.m_event.type)
			{
			case SDL_QUIT:
			{
				quit = true;
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				xStart = handler.m_event.button.x / SDL_Handler::CELL_WIDTH;
				yStart = handler.m_event.button.y / SDL_Handler::CELL_HEIGHT;
				clickedOn = game->getFieldPos(xStart, yStart);
				if (clickedOn)
				{
					if (clickedOn->getTeam() == game->getTurn())
					{
						game->renderPossibleMoves(clickedOn);
					}
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				if (clickedOn != nullptr)
				{
					if (clickedOn->getTeam() == game->getTurn())
					{
						game->undoRenderPossibleMoves(clickedOn);
					}
				}
				xEnd = handler.m_event.button.x / SDL_Handler::CELL_WIDTH;
				yEnd = handler.m_event.button.y / SDL_Handler::CELL_HEIGHT;
				if (clickedOn != nullptr)
				{
					if ((xStart != -1 && yStart != -1 && xEnd != -1 && yEnd != -1)
						&& (clickedOn->getTeam() == game->getTurn())
						&& (game->isValidMove(xEnd, yEnd, clickedOn)))
					{
						std::vector<PossibleMove> list = game->getFieldPos(xStart, yStart)->getPossibleMoves();
						for (const auto& [XCoord, YCoord, MoveType] : list)
						{
							if (XCoord == xEnd && YCoord == yEnd)
							{
								game->move(clickedOn, PossibleMove{ xEnd, yEnd, MoveType });
							}
						}
						xStart = -1;
						yStart = -1;
						yEnd = -1;
						game->calcAllMoves();
						clickedOn = nullptr;
					}
				}
				break;
			}
			}
		}
	}

	handler.cleanUp();
}