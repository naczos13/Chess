#include <SDL.h>
#include <stdio.h>
#include "MainLoop.h"
#include "SDL_Handler.h"
#include "Game.h"
#include <memory>

void MainLoop::run()
{
	SDL_Handler handler = SDL_Handler();
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
			moveStart.x = handler.m_event.button.x / handler.CELL_WIDTH;
			moveStart.y = handler.m_event.button.y / handler.CELL_HEIGHT;
			clickedPiece = game.getPieceByPosition(moveStart.x, moveStart.y);
			if (clickedPiece)
			{
				if (clickedPiece->getTeam() == game.getTurn())
				{
					possibleMoves = clickedPiece->getPossibleMoves(game.m_board);
					handler.renderPossibleMoves(game.getPieces(), possibleMoves);
				}
				else
				{
					clickedPiece = nullptr;
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if (clickedPiece)
			{
				// Clear the board from the possible moves
				handler.refreshTheWindow(game.getPieces());

				moveEnd.x = handler.m_event.button.x / handler.CELL_WIDTH;
				moveEnd.y = handler.m_event.button.y / handler.CELL_HEIGHT;

				if (auto move = game.GetValidMove(moveEnd, possibleMoves))
				{
					game.move(*move);
				}
			}
			clickedPiece = nullptr;
			break;
		}
		}
	}
}