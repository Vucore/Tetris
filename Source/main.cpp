#include "SDL2/SDL.h"
#include "Board.h"
#include <iostream>
#include <fstream>
#include "Game.h"
#include "IO.h"

using namespace std;

Pieces* CreatePieces(int& TotalPieces);
void PlayGame(Board* _board, Pieces* _pieces, IO* _ioService, Game *_game);

int main(int argc, char* args[])
{
	int TotalPieces;
    Pieces *pieces = CreatePieces(TotalPieces);
	IO IOService;
	IOService.Music("Music//MusicBG.mp3", -1, false);
	if(IOService.Menu() == 0)
		return 0;
	IOService.Music("Music//MusicBG.mp3", -1, true);
	restart:
	Board board(pieces);
	Game game(&board, pieces, &IOService, TotalPieces);
	PlayGame(&board, pieces, &IOService, &game);	
	if(board.HightScore <= board.GetScore())
		board.HightScore = board.GetScore();

	game.EndGame();
    IOService.Music("Music//GameOver.mp3", 0, false);
	if(IOService.ReStart())
		goto restart;
	Mix_CloseAudio();
    delete pieces;
    return 0;
}

Pieces* CreatePieces(int& TotalPieces)
{
	ifstream Infile;
	Infile.open("Pieces//Pieces.txt");
	Infile >> TotalPieces;
    Pieces *pieces = new Pieces(TotalPieces, TotalRotation, Start);
	Infile >> *pieces;
    Infile.close();
	return pieces;
}

void PlayGame(Board* _board, Pieces* _pieces, IO* _ioService, Game *_game)
{
	unsigned long timeBefore = SDL_GetTicks(); // ms
	int temp = _board->GetScore();
	while (!_board->IsGameOver())
	{
		if(temp < _board->GetScore())
		{
			_ioService->Music("Music//Eat.mp3", 0, false);
			temp = _board->GetScore();
		}
		_ioService->ClearScreen();
		_game->DrawAll();
		_ioService->UpdateScreen();
		// move
		int keyPressed = _ioService->GetKeyPress();
		switch (keyPressed)
		{
		case SDLK_LEFT:
			if (_board->IsLegalMove(_game->CurrentPiece, _game->CurrentPieceRotation, _game->CurrentPiecePosX - 1, _game->CurrentPiecePosY))
				_game->CurrentPiecePosX--;
			break;
		case SDLK_RIGHT:
			if (_board->IsLegalMove(_game->CurrentPiece,_game->CurrentPieceRotation, _game->CurrentPiecePosX + 1, _game->CurrentPiecePosY))
				_game->CurrentPiecePosX++;
			break;
		case SDLK_DOWN:
			if (_board->IsLegalMove(_game->CurrentPiece,_game->CurrentPieceRotation, _game->CurrentPiecePosX, _game->CurrentPiecePosY + 1))
				_game->CurrentPiecePosY++;
			break;
		case SDLK_x:
			if (_board->IsGameOver())
				break;
			_game->DropPiece();
			break;
		case SDLK_z:
				_game->RotatePiece();
			break;
		}
		
		unsigned long timeAfter = SDL_GetTicks();
		if (timeAfter - timeBefore > _board->GetWaitTime())
		{
		 	if(_board->IsLegalMove(_game->CurrentPiece, _game->CurrentPieceRotation, _game->CurrentPiecePosX, _game->CurrentPiecePosY + 1))
		 	{
                _game->CurrentPiecePosY++;
		 	}
		     else
		 	{
		 		_board->StorePieces(_game->CurrentPiece, _game->CurrentPieceRotation, _game->CurrentPiecePosX, _game->CurrentPiecePosY);
				_board->DeleteCompletedLines();
		 		_game->CreatePiece();
				_game->WriteScore();
		 	}
		 	timeBefore = SDL_GetTicks();
		}
	}
}
