#include "Pieces.h"
// #include <fstream>

Pieces::Pieces(int x, int y, matrix m)
    :Point<matrix>(x, y, m)
{ }

Pieces::~Pieces()
{ }

ostream& operator<<(ostream& o, const Pieces& P)
{
    for (int i = 0; i < P.r; i++)
    {
        for (int j = 0; j < P.c; j++)
        {
            o << *(*(P.p + i) + j);
        }
    }
    return o;
}

matrix& Pieces::Rotation(int PiecesCurrent, int RotationCurrent)
{
    return *(*(this->p + PiecesCurrent) + RotationCurrent);
}

int Pieces::GetValue(const int pieceType, const int rotation, const int xIndex, const int yIndex)
{
	return (*(this->p + pieceType) + rotation)->M[xIndex][yIndex];
}

istream& operator>>(istream& is, Pieces& _pieces)
{
    for (int i = 0; i < _pieces.r; i++)
    {
        for (int j = 0; j < _pieces.c; j++)
        {
            is >> *(*(_pieces.p + i) + j);
        }
    }
    return is;
}