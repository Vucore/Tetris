#pragma once
#include "Pieces.h"
#include <iostream>
using namespace std;

const int PIECE_BLOCK_SIZE = 5;
const int SCREEN_WIDTH = 1280;//640;
const int SCREEN_HEIGHT = 720;//480;
const int BLOCK_SIZE = 25;//16;
const int BOARD_WIDTH_BLOCKS = 15;
const int BOARD_HEIGHT_BLOCKS = 25;
const int BOARD_SEPERATOR_THICKNESS = 6;
const int BOARD_SEPERATOR_MARGIN = SCREEN_WIDTH/2 - (BOARD_WIDTH_BLOCKS * BLOCK_SIZE)/2 - BOARD_SEPERATOR_THICKNESS;

class Board : public Point<int>
{
private:
    Pieces* _pieces;
    void DeleteLine(const int);
    int Score;
    int WAIT_TIME;
public:
    Board(Pieces* defaultPiece = nullptr);
    ~Board();
    static int HightScore;
    int GetBoardPosXInPixels(const int posX);
	int GetBoardPosYInPixels(const int posY);
    //int GetBoardState(const int, const int);
    void StorePieces(const int PiecesType, const int Rotation, const int PosX, const int PosY);
    friend ostream& operator<<(ostream&, const Board&);
    bool IsLegalMove(const int pieceType, const int Rotation, const int posX, const int posY);
    void DeleteCompletedLines();
    bool IsGameOver();
    int GetScore();
    void IncreaseScore();
    int GetWaitTime();
};
