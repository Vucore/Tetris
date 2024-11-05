#pragma once
// #include "matrix.h"
#include "Point.cpp"
#include <iostream>
using namespace std;


class Pieces : public Point<matrix>
{
public:
    Pieces(int = 0, int = 0, matrix = Start); 
    ~Pieces();
    friend ostream& operator<<(ostream&, const Pieces&);
    matrix& Rotation(int = 0, int = 0);
    //int GetBlockType(const int pieceType, const int rotation, const int xIndex, const int yIndex);
    int GetValue(const int pieceType, const int rotation, const int xIndex, const int yIndex);
    friend istream& operator>>(istream&, Pieces&);
};

