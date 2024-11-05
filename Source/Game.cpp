#include <stdlib.h>
#include "Game.h"

Game::Game(Board* board, Pieces* pieces, IO* ioService, int TotalPieces)
{
	_board = board;
	_pieces = pieces;
	_ioService = ioService;
	_totalPieces = TotalPieces;
	InitializeGame();
}

Game::~Game()
{ }

void Game::DrawAll()
{
	this->ShowImage();
	this->DrawBoard();
	this->DrawStoredPieces();
	this->DrawPiece(CurrentPiece, CurrentPiecePosX, CurrentPiecePosY, CurrentPieceRotation);
	this->DrawFuturePiece();
	this->DrawPiece(_nextPiece, _nextPiecePosX, _nextPiecePosY, _nextPieceRotation);
    this->WriteScore();
}

void Game::InitializeGame()
{
	this->_nextPiece = GetRandomInt(0, this -> _totalPieces - 1);
	this->_nextPiecePosX = BOARD_WIDTH_BLOCKS  + 3 + (2 * BOARD_SEPERATOR_THICKNESS)/16 + 2 + 2/16;
	this->_nextPiecePosY = 4;
	this->_nextPieceRotation = GetRandomInt(0, TotalRotation - 1);
	this->CreatePiece();
}

void Game::CreatePiece()
{
	this->CurrentPiece = _nextPiece;
	this->CurrentPiecePosX = (BOARD_WIDTH_BLOCKS / 2) - 2;
	this->CurrentPiecePosY = -1;
	this->CurrentPieceRotation = _nextPieceRotation;
	this->_nextPiece = GetRandomInt(0, this -> _totalPieces - 1);
}

void Game::RotatePiece()
{
    if (CurrentPieceRotation < TotalRotation - 1)
    {
        if (_board->IsLegalMove(CurrentPiece, CurrentPieceRotation + 1, CurrentPiecePosX, CurrentPiecePosY))
            this->CurrentPieceRotation++;
    }
    else
    {
        if (_board->IsLegalMove(CurrentPiece, 0, CurrentPiecePosX, CurrentPiecePosY))
            this->CurrentPieceRotation = 0;
    }
}

void Game::DropPiece()
{
    int yMax = GetPieceMaxPosY();
    
    this->CurrentPiecePosY += yMax;
    _board->StorePieces(CurrentPiece, CurrentPieceRotation, CurrentPiecePosX, CurrentPiecePosY);
    _board->DeleteCompletedLines();
    this->CreatePiece();
}

Color Game::GetPieceTypeColor(const int pieceType)
{
	switch (pieceType)
	{
	case 0:
		return YELLOW;
	case 1:
		return RED;
	case 2:
		return CYAN;
	case 3:
		return AMBER;
	case 4:
		return MAGENTA;
	case 5:
		return ORANGE;
	case 6:
		return GREEN;
	default:
		return BLUE;
	}
}

void Game::DrawBoard()
{
	// vẽ cột trái
	_ioService->DrawRectangle(BOARD_SEPERATOR_MARGIN,
		SCREEN_HEIGHT - (BOARD_HEIGHT_BLOCKS * BLOCK_SIZE),
		BOARD_SEPERATOR_THICKNESS,
		BOARD_HEIGHT_BLOCKS * BLOCK_SIZE,
		GRAY);
    // vẽ cột phải
	_ioService->DrawRectangle(SCREEN_WIDTH - BOARD_SEPERATOR_MARGIN - BOARD_SEPERATOR_THICKNESS,
		SCREEN_HEIGHT - BOARD_HEIGHT_BLOCKS * BLOCK_SIZE,
		BOARD_SEPERATOR_THICKNESS,
		BOARD_HEIGHT_BLOCKS * BLOCK_SIZE,
		GRAY);
    // vẽ lưới ngang
	for (int i = 0; i <= BOARD_HEIGHT_BLOCKS; i++)
	{
		_ioService->DrawRectangle(BOARD_SEPERATOR_MARGIN + BOARD_SEPERATOR_THICKNESS,
			SCREEN_HEIGHT - BOARD_HEIGHT_BLOCKS * BLOCK_SIZE + i * BLOCK_SIZE - 1,
			BOARD_WIDTH_BLOCKS * BLOCK_SIZE ,
			1 , GRAY);
	}
	//vẽ lưới dọc
	for (int j = 0; j <= BOARD_WIDTH_BLOCKS; j++)
	{
		_ioService->DrawRectangle(BOARD_SEPERATOR_MARGIN + BOARD_SEPERATOR_THICKNESS + j * BLOCK_SIZE,
			SCREEN_HEIGHT - BOARD_HEIGHT_BLOCKS * BLOCK_SIZE,
			1 , BOARD_HEIGHT_BLOCKS * BLOCK_SIZE, GRAY);
	}
	// vẽ chỉ đỏ
	_ioService->DrawRectangle(BOARD_SEPERATOR_MARGIN + BOARD_SEPERATOR_THICKNESS,
		SCREEN_HEIGHT - BOARD_HEIGHT_BLOCKS * BLOCK_SIZE + 4 * BLOCK_SIZE - 1,
		BOARD_WIDTH_BLOCKS * BLOCK_SIZE,
		1,
		RED);
	// hightscore
	for (int i = 0; i < BOARD_SEPERATOR_THICKNESS ; i++)
	{   
		_ioService->DrawRectangleOutline(BOARD_SEPERATOR_MARGIN - 16 * BLOCK_SIZE - i,
			SCREEN_HEIGHT - 16 * BLOCK_SIZE - i,
			14 * BLOCK_SIZE + 2 * i,
			2 * BLOCK_SIZE +2 * i,
			RED);
	}
	// vẽ ô khối tiếp theo
	for (int i = 0; i < BOARD_SEPERATOR_THICKNESS ; i++)
	{   
		_ioService->DrawRectangleOutline(BOARD_SEPERATOR_MARGIN + 3 * BOARD_SEPERATOR_THICKNESS + (BOARD_WIDTH_BLOCKS + 3) * BLOCK_SIZE - i,
			SCREEN_HEIGHT - BOARD_HEIGHT_BLOCKS * BLOCK_SIZE + 4 * BLOCK_SIZE - i,
			9 * BLOCK_SIZE + 2 * i,
			6 * BLOCK_SIZE +2 * i,
			GRAY);
	}
	// vẽ ô điểm
	for (int i = 0; i < BOARD_SEPERATOR_THICKNESS ; i++)  
	{
		_ioService->DrawRectangleOutline(BOARD_SEPERATOR_MARGIN + 3 * BOARD_SEPERATOR_THICKNESS + (BOARD_WIDTH_BLOCKS + 3) * BLOCK_SIZE - i,
			SCREEN_HEIGHT - BOARD_SEPERATOR_THICKNESS - 8 * BLOCK_SIZE - i,
			9 * BLOCK_SIZE + 2 * i,
			3 * BLOCK_SIZE + 2 * i,
			GRAY);
	}
}

void Game::DrawPiece(const int pieceType, const int posX, const int posY, const int rotation)
{
	Color color = GetPieceTypeColor(pieceType);

	int posXPixels = _board->GetBoardPosXInPixels(posX);
	int posYPixels = _board->GetBoardPosYInPixels(posY) - 1;

	for (int i = 0; i < PIECE_BLOCK_SIZE; i++) // hàng
		for (int j = 0; j < PIECE_BLOCK_SIZE; j++)  // cột
			if (_pieces->GetValue(pieceType, rotation, i, j) != 0)
			{
				_ioService->DrawRectangle(posXPixels + (j * BLOCK_SIZE), posYPixels + (i * BLOCK_SIZE), BLOCK_SIZE - 1, BLOCK_SIZE - 1, color);
				_ioService->DrawRectangleOutline(posXPixels + (j * BLOCK_SIZE), posYPixels + (i * BLOCK_SIZE), BLOCK_SIZE - 1, BLOCK_SIZE - 1, WHITE);
			}			
}

void Game::DrawStoredPieces()
{
	int boardState;
	Color color;

	for (int i = 0; i < BOARD_HEIGHT_BLOCKS; i++)
	{
		for (int j = 0; j < BOARD_WIDTH_BLOCKS; j++)
		{
			boardState = _board->GetValue(j, i);
			if (boardState != -1)
			{
				color = GetPieceTypeColor(boardState);
				_ioService->DrawRectangle(BOARD_SEPERATOR_MARGIN + BOARD_SEPERATOR_THICKNESS + (j * BLOCK_SIZE) + 1,
					SCREEN_HEIGHT - (BOARD_HEIGHT_BLOCKS * BLOCK_SIZE) + (i * BLOCK_SIZE),
					BLOCK_SIZE - 1, BLOCK_SIZE - 1, color);
			}
		}
	}
}

int Game::GetPieceMaxPosY()
{
    int y = 0;
    while (_board->IsLegalMove(CurrentPiece,CurrentPieceRotation, CurrentPiecePosX, CurrentPiecePosY + y))
        y++;
    
    return y - 1;
}

void Game::DrawFuturePiece()
{
	int yMax = GetPieceMaxPosY();
	Color color = YELLOW;
	
	int PosXPixel = _board->GetBoardPosXInPixels(CurrentPiecePosX);
    int PosYPixel = _board->GetBoardPosYInPixels(CurrentPiecePosY + yMax);
	for(int i = 0; i < PIECE_BLOCK_SIZE; i++) //hang
	{
		for(int j = 0; j < PIECES_BLOCK_SIZE; j++) //cot
		{
			if(_pieces->GetValue(CurrentPiece, CurrentPieceRotation, i, j) != 0)
			{
                _ioService->DrawRectangleOutline(PosXPixel + (j*BLOCK_SIZE), PosYPixel + (i*BLOCK_SIZE), BLOCK_SIZE - 1, BLOCK_SIZE -1, color);
			}
		}
	}
}

int Game::GetRandomInt(const int min, const int max)
{
	return min + (rand() % (max - min + 1));
}

void Game::ShowImage()
{
	_ioService->Image("image//BackGround.jpg",0 ,0 ,SCREEN_WIDTH ,SCREEN_HEIGHT);
	_ioService->Image("image//TetrisLogo.png", (BOARD_SEPERATOR_MARGIN - 330)/2,
											   10,
											   330,
											   230); //edit
	_ioService->Image("image//Score.png", BOARD_SEPERATOR_MARGIN + 3 * BOARD_SEPERATOR_THICKNESS + BLOCK_SIZE * BOARD_WIDTH_BLOCKS - 2 * BLOCK_SIZE - 10, 
							       SCREEN_HEIGHT - BOARD_HEIGHT_BLOCKS * BLOCK_SIZE + 10 * BLOCK_SIZE + 10, 
								   480, 
								   270);
	_ioService->Image("image//NextPiece.png", BOARD_SEPERATOR_MARGIN + 3 * BOARD_SEPERATOR_THICKNESS + BLOCK_SIZE * BOARD_WIDTH_BLOCKS - 2 * BLOCK_SIZE + 5,
									   SCREEN_HEIGHT - BOARD_HEIGHT_BLOCKS * BLOCK_SIZE - 2 * BLOCK_SIZE - 20,
									   480,
									   270);
}
void Game::WriteScore()
{
	SDL_Color colorhightscore = { 255, 0, 0 };
	_ioService->WriteText("Hight Score:", 50, BOARD_SEPERATOR_MARGIN - 13 * BLOCK_SIZE,
											  SCREEN_HEIGHT - 15 * BLOCK_SIZE - 8,
											  colorhightscore);
	SDL_Color hightscore = { 255, 255, 0 };
	_ioService->WriteText(to_string(_board->HightScore), 80, BOARD_SEPERATOR_MARGIN - 6 * BLOCK_SIZE,
															 SCREEN_HEIGHT - 15 * BLOCK_SIZE - 10,
															 hightscore);
	
	SDL_Color colorscore = { 255, 165, 0 }; 
	_ioService->WriteText(to_string(_board->GetScore()), 100, BOARD_SEPERATOR_MARGIN + 3 * BOARD_SEPERATOR_THICKNESS + BLOCK_SIZE * (BOARD_WIDTH_BLOCKS + 3 + 4) + BLOCK_SIZE/2,
													      SCREEN_HEIGHT - BOARD_SEPERATOR_THICKNESS - (8 - 3/2) * BLOCK_SIZE, colorscore);   
	SDL_Color colortext1 = { 187, 255, 255 };
	_ioService->WriteText("How to Play", 90, BOARD_SEPERATOR_MARGIN - 9 * BLOCK_SIZE, 
											 SCREEN_HEIGHT - 10 * BLOCK_SIZE, colortext1);
	_ioService->WriteText("Press X: Send block to known location", 40, BOARD_SEPERATOR_MARGIN - 9 * BLOCK_SIZE, 
																	   SCREEN_HEIGHT - 8 * BLOCK_SIZE, colortext1);
	_ioService->WriteText("Press Z: Rotate the current block", 40, BOARD_SEPERATOR_MARGIN - 9 * BLOCK_SIZE, 
																   SCREEN_HEIGHT - 7 * BLOCK_SIZE + 3, colortext1);
}

void Game::EndGame()
{
	_ioService->DrawRectangle(SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 150, 600, 350, CYAN);
	_ioService->DrawRectangleOutline(SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 150, 600, 350, RED);
	
	SDL_Color colorGO = {130, 130, 130};
	_ioService->WriteText("Game Over", 200, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 100, colorGO);
	_ioService->WriteText("Press R: Restart Game", 50, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 110, colorGO);
	_ioService->WriteText("Press Q: Quit Game", 50, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 140, colorGO);
	
	SDL_Color colorSC = { 255, 165, 0 };
	_ioService->WriteText("Your Score:", 90, SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2, colorSC);
	_ioService->WriteText(to_string(_board->GetScore()), 100, SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 50, colorSC);
	
	SDL_Color colorHS = { 255, 0, 0 };
	_ioService->WriteText("Hight Score:", 90, SCREEN_WIDTH/2 + 150, SCREEN_HEIGHT/2, colorHS);
    _ioService->WriteText(to_string(_board->HightScore), 100, SCREEN_WIDTH/2 + 150, SCREEN_HEIGHT/2 + 50, colorHS);
	_ioService->UpdateScreen();
}