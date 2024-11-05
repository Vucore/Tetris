#include "matrix.h"
// #include <iostream>
// using namespace std;

matrix :: matrix()
{
    for (int i = 0; i < PIECES_BLOCK_SIZE; i++)
    {
        for (int j = 0; j < PIECES_BLOCK_SIZE; j++)
        {
            this->M[i][j] = 0;
        }
        
    }
    
}

matrix :: ~matrix()
{

}

ostream& operator<<(ostream& o, const matrix& m)
{
    for (int i = 0; i < PIECES_BLOCK_SIZE; i++)
    {
        for (int j = 0; j < PIECES_BLOCK_SIZE; j++)
        {
            o << m.M[i][j] << " ";
        }
        o << endl;
    }
    return o;
}

istream& operator>>(istream& is, matrix& m)
{
    for (int i = 0; i < PIECES_BLOCK_SIZE; i++)
    {
        for (int j = 0; j < PIECES_BLOCK_SIZE; j++)
        {
            is >> m.M[i][j];
        }
    }
    return is;
}

matrix& matrix::operator=(const matrix& m)
{
    for (int i = 0; i < PIECES_BLOCK_SIZE; i++)
    {
        for (int j = 0; j < PIECES_BLOCK_SIZE; j++)
        {
            this->M[i][j] = m.M[i][j];
        }
    }
    return *this;
}

// int matrix::operator[][](int x, int y)
// {
//     return this->M[x][y];
// }

// int* matrix::operator[](int index) {

//     return this->M[index];

// }