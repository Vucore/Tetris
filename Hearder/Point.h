#pragma once
#include "matrix.h"

const matrix Start;

template<typename T>
class Point
{
protected:
    int r, c;
    T **p;
public:
    Point(int = 0, int = 0,  T defaultValue = T());
    ~Point();
    template <typename T1>
    friend istream& operator>>(istream&, Point<T1>&);
    virtual T GetValue(const int x, const int y);
};
