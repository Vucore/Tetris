#include "Board.h"
int Board::HightScore = 0;

Board::Board(Pieces* pieces)
    : Point<int>(BOARD_HEIGHT_BLOCKS, BOARD_WIDTH_BLOCKS, -1)
{ 
    this->_pieces = pieces;
	this->Score = 0;
	this->WAIT_TIME = 700;
}

Board::~Board()
{ }

void Board::DeleteLine(int const PosY)
{
    for (int i = PosY; i > 0; i--)
        for (int j = 0; j < BOARD_WIDTH_BLOCKS; j++)
        {
            *(*(this->p + i) + j) = *(*(this-> p + i - 1) + j);
        }
}

int Board::GetBoardPosXInPixels(const int posX)
{
	return (posX * BLOCK_SIZE) + BOARD_SEPERATOR_MARGIN + BOARD_SEPERATOR_THICKNESS + 1;
}

int Board::GetBoardPosYInPixels(const int posY)
{
	return (posY * BLOCK_SIZE) + SCREEN_HEIGHT - (BOARD_HEIGHT_BLOCKS * BLOCK_SIZE) + 1;
}

void Board::StorePieces(const int PiecesType, const int Rotation, const int PosX, const int PosY)
{
    int boardPosX = PosX;
	int boardPosY = PosY;

	for (int i = 0; i < PIECE_BLOCK_SIZE; i++)
	{
		boardPosX = PosX;
		for (int j = 0; j < PIECE_BLOCK_SIZE; j++)
		{
			if (_pieces->GetValue(PiecesType, Rotation, i, j) != 0)
            {
               *(*(this->p + boardPosY) + boardPosX)= PiecesType;
            }
			boardPosX++;
		}
		boardPosY++;
	}
}

ostream& operator<<(ostream& o, const Board& b)
{
    for (int i = 0; i < BOARD_HEIGHT_BLOCKS; i++)
    {
        for (int j = 0; j < BOARD_WIDTH_BLOCKS; j++)
        {
            o << *(*(b.p + i) + j) << " ";
        }
        o << endl;
    }
    return o;
}

bool Board::IsLegalMove(const int pieceType, const int Rotation, const int posX, const int posY)
{
	int boardPosX = posX;
	int boardPosY = posY;

	for (int i = 0; i < PIECE_BLOCK_SIZE; i++)
	{
		boardPosX = posX;
		for (int j = 0; j < PIECE_BLOCK_SIZE; j++)
		{
			if (this->_pieces->GetValue(pieceType, Rotation, i, j) != 0)
			{
				if (boardPosX < 0 || boardPosX > BOARD_WIDTH_BLOCKS - 1 || boardPosY > BOARD_HEIGHT_BLOCKS - 1) // viền bảng
					return false;

				if (posY >= 0 && *(*(this->p + boardPosY) + boardPosX) != -1)  // va chạm khối có sẵn
					return false;
			}
			boardPosX++;
		}
		boardPosY++;
	}
	return true;
}

void Board::DeleteCompletedLines()
{
    for (int i = 0; i < BOARD_HEIGHT_BLOCKS; i++)
    {
        for (int j = 0; j < BOARD_WIDTH_BLOCKS; j++)
        {
            if (*(*(this->p + i) + j) == -1)
			{
				break;
			}
            else
			{
				if (j == BOARD_WIDTH_BLOCKS - 1)
				{	
					DeleteLine(i);				
					this->Score += 1;
					if(this->Score % 5 == 4 && WAIT_TIME > 100)
					{
						this->WAIT_TIME = this->WAIT_TIME - 30;
					}
				} 
			}  
        }
    }
}

bool Board::IsGameOver()
{
 	for (int j = 0; j < BOARD_WIDTH_BLOCKS ; j++)
		if (*(*(this ->p + 4 ) +j) != -1)
 			return true;

 	return false;
}

int Board::GetScore()
{
	return this->Score;
}

void Board::IncreaseScore()
{
	this->Score++;
}
int Board::GetWaitTime()
{
	return this->WAIT_TIME;
}