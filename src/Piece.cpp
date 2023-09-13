#include "Piece.h"
#include "King.h"
#include <string>
#include <iostream>

int CoordToIndex(int x, int y) {
	return x * 8 + y;
}

int CoordToIndex(Point p) {
	return p.x * 8 + p.y;
}

bool Piece::moveMakeMyKingToBeCheck(Piece** board, const King* king, const Point* move, Piece* CurrentPiece) const
{
	// make this move, loop over all the enemy pieces and check if the can checkmate the king, then revert the move

	// what I need
	// - make a move
	// - list of enemy Pieces
	//		- 
	// - function that check if piece can capture a king
	// - revert the move

	bool captureTheKing = false;

	// # Make a move
	Piece* capturePiece = board[CoordToIndex(move->x, move->y)];
	const Point prevPiecePosition = CurrentPiece->getPos();
	CurrentPiece->setPosition(*move);
	board[CoordToIndex(move->x, move->y)] = CurrentPiece;
	board[CoordToIndex(prevPiecePosition.x, prevPiecePosition.y)] = nullptr;

	for (int x{ 0 }; x < 8; x++)
	{
		for (int y{ 0 }; y < 8; y++)
		{
			const Piece* potentialEnemy = board[CoordToIndex(x, y)];
			if (potentialEnemy && potentialEnemy->getTeam() != king->getTeam())
			{
				if (potentialEnemy->canEliminateKing(board, king))
				{
					// revert the move
					board[CoordToIndex(move->x, move->y)] = capturePiece;
					CurrentPiece->setPosition(prevPiecePosition);
					board[CoordToIndex(prevPiecePosition.x, prevPiecePosition.y)] = CurrentPiece;
					return true;
				}
			}
		}
	}

	// revert the move
	board[CoordToIndex(move->x, move->y)] = capturePiece;
	CurrentPiece->setPosition(prevPiecePosition);
	board[CoordToIndex(prevPiecePosition.x, prevPiecePosition.y)] = CurrentPiece;
	return false;
}

bool Piece::canEliminateKing(Piece** board, const Piece* king) const
{
	std::vector<Point> posible_positions = getPhysicallyPossiblePositions(board);
	for (const auto& [x, y] : posible_positions)
	{
		const Piece* possiblePiece = board[CoordToIndex(x, y)];
		if (possiblePiece == king)
			return true;
	}
	return false;
}

std::vector<PossibleMove> Piece::getPossibleMoves(Piece** board, bool checkCheck)
{
	return calcPossibleMoves(board, checkCheck);
}

std::vector<PossibleMove> Piece::simulateMove(std::vector<PossibleMove> moveList,
											PossibleMove move,
											King* king,
											Piece** board,
											bool checkCheck) const
{
	if (!checkCheck)
	{
		moveList.push_back(move);
	}
	else
	{
		bool enemyPlace = true;
		king->setCheck(board, king->getPos().x, king->getPos().y);
		Piece* zwisch = board[CoordToIndex(move.XCoord, move.YCoord)];
		enemyPlace = false;

		if (board[CoordToIndex(move.XCoord, move.YCoord)] != nullptr)
		{
			enemyPlace = true;
			board[CoordToIndex(move.XCoord, move.YCoord)] = nullptr;
		}

		std::swap(board[CoordToIndex(move.XCoord, move.YCoord)], board[CoordToIndex(m_pos.x, m_pos.y)]);
		if (m_type == KING)
		{
			king->setCheck(board, move.XCoord, move.YCoord);
		}
		else
		{
			king->setCheck(board, king->getPos().x, king->getPos().y);
		}
		std::swap(board[CoordToIndex(move.XCoord, move.YCoord)], board[CoordToIndex(m_pos.x, m_pos.y)]);

		if (enemyPlace)
		{
			board[CoordToIndex(move.XCoord, move.YCoord)] = zwisch;
		}
		if (!king->getCheck())
		{
			moveList.push_back(move);
		}
		king->setCheck(board, king->getPos().x, king->getPos().y);
	}
	return moveList;
}

King* Piece::getOwnKing(Piece** board) const
{
	for  (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[CoordToIndex(i, j)] != nullptr)
			{
				if (board[CoordToIndex(i, j)]->getTeam() == m_team && board[CoordToIndex(i, j)]->getType() == PieceType::KING)
				{
					King* ret = (King*)(board[CoordToIndex(i, j)]);
					return ret;
				}
			}
		}
	}
	return nullptr;
}

Piece::Piece(Team team, Point pos, SDL_Handler* handler, PieceType type)
	:m_team(team), m_pos(pos), m_handler(handler), m_texture(NULL), m_hasMoved(false), m_type(type)
{
	std::cout << "Created Piece: count: " << ++Piece::s_piece_counter << std::endl;
}

Piece::Piece()
{
	std::cout << "Created Empty Piece: count: " << ++Piece::s_piece_counter << std::endl;
}

Piece::Piece(const Piece& piece)
	:m_team(piece.m_team), m_pos(piece.m_pos), m_handler(piece.m_handler), m_texture(NULL), m_hasMoved(false), m_type(piece.m_type)
{
	std::cout << "Copy Piece" << std::endl;
}

Piece::~Piece()
{
	SDL_DestroyTexture(m_texture);

	m_handler->undoPieceRender(m_pos.x, m_pos.y);
}


void Piece::render()
{
	SDL_Rect src = {0, 0, 60, 60};
	if (m_handler == nullptr)
	{
		sayMyName();
	}
	SDL_Rect dest = { m_handler->SCREEN_WIDTH / 8 * m_pos.x - 2,
					  m_handler->SCREEN_HEIGHT / 8 * m_pos.y,
					  m_handler->SCREEN_WIDTH / 8,
					  m_handler->SCREEN_HEIGHT / 8 };
	m_handler->DrawRectangle(src, dest, m_texture);
}

void Piece::sayMyName()
{
	std::cout << "No Piece here" << std::endl;
}

int Piece::s_piece_counter = 0;

