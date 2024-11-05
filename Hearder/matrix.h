#pragma once
#include <iostream>
using namespace std;

const int PIECES_BLOCK_SIZE = 5;

class matrix
{
public:
    int M[PIECES_BLOCK_SIZE][PIECES_BLOCK_SIZE];
public:
    matrix();
    ~matrix();
    friend ostream& operator<<(ostream&, const matrix&);
    friend istream& operator>>(istream&, matrix&);
    matrix& operator=(const matrix&);
    // int* operator[](int index);
};
