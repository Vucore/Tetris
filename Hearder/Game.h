#pragma once

#include "IO.h"
#include "Board.h"
#include "Pieces.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int TotalRotation = 4;

class Game
{
public:
	Game(Board* board, Pieces* pieces, IO* ioService, int TotalPieces);
	~Game();

	int CurrentPiece;
	int CurrentPiecePosX;
	int CurrentPiecePosY;
	int CurrentPieceRotation;

	void DrawAll(); 
	void CreatePiece();//r
	void RotatePiece();
    void DropPiece();
	void WriteScore();
	void EndGame();
private:

	Board* _board;
	Pieces *_pieces;
	IO* _ioService;

	int _nextPiece;
	int _nextPiecePosX;
	int _nextPiecePosY;
	int _nextPieceRotation;
	int _totalPieces;

	void InitializeGame();
	Color GetPieceTypeColor(const int pieceType); 
	void DrawBoard(); 
	void DrawPiece(const int pieceType, const int posX, const int posY, const int rotation); 
	void DrawStoredPieces();
	int GetPieceMaxPosY();
	void DrawFuturePiece();
	int GetRandomInt(const int min, const int max);
	void ShowImage();
};
