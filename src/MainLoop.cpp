#include <SDL.h>
#include <stdio.h>
#include "MainLoop.h"
#include "SDL_Handler.h"
#include "Game.h"
#include <memory>

void MainLoop::run()
{
	SDL_Handler handler = SDL_Handler();

	handler.renderBackground();

	Game game = Game(&handler);

	Point moveStart;
	Point moveEnd;
	Piece* clickedPiece{};
	std::vector<PossibleMove> possibleMoves;

	while (SDL_WaitEvent(&handler.m_event))
	{
		switch (handler.m_event.type)
		{
		case SDL_QUIT:
		{
			return;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			moveStart.x = handler.m_event.button.x / SDL_Handler::CELL_WIDTH;
			moveStart.y = handler.m_event.button.y / SDL_Handler::CELL_HEIGHT;
			clickedPiece = game.getPieceByPosition(moveStart.x, moveStart.y);
			if (clickedPiece)
			{
				if (clickedPiece->getTeam() == game.getTurn())
				{
					possibleMoves = game.getPosibleMovesForPiece(clickedPiece);
					game.renderPossibleMoves(possibleMoves);
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{

			if (clickedPiece)
			{
				// Clear the board from the possible moves
				game.undoRenderPossibleMoves(possibleMoves);

				moveEnd.x = handler.m_event.button.x / SDL_Handler::CELL_WIDTH;
				moveEnd.y = handler.m_event.button.y / SDL_Handler::CELL_HEIGHT;

				if (auto move = game.GetValidMove(moveEnd, possibleMoves))
				{
					game.move(clickedPiece, *move);
					game.calcAllMoves();
				}
			}
			break;
		}
		}
		
	}
}