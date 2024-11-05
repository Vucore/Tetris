// #pragma once
#include "Point.h"

template<typename T>
Point<T>::Point(int r, int c, T S)
    : r(r), c(c)
{
    this->p = new T *[r];
    for (int i = 0; i < this->r; i++)
    {
        *(this->p + i) = new T [this ->c];
        for (int j = 0; j < this->c; j++)
        {
            *(*(this->p + i) + j) = S;
        }
    }
}

template<typename T>
Point<T>::~Point()
{
    for (int i = 0; i < this->r; i++)
    {
        delete[] *(this->p + i);
    }
    delete[] this->p;
}

template<typename T1>
istream& operator>>(istream& is, Point<T1>& t)
{
    
    for (int i = 0; i < t.r; i++)
    {
        for (int j = 0; j < t.c; j++)
        {
            is >> *(*(t.p + i) + j);
        }
    }
    return is;
}

template<typename T>
T Point<T>::GetValue(const int x, const int y)
{
    return *(*(this->p + y) + x);
}