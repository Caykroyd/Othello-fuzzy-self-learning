#ifndef MATRIX_H
#define MATRIX_H
#define B_SIZE 6

#include <iostream>
using namespace std;

class Matrix
{
    public:
        Matrix();
        ~Matrix();
        Matrix(const Matrix& other);

        void Reset();

        void Print(string zero, string pos, string neg);
        int cell[B_SIZE][B_SIZE];
    protected:

    private:
};

#endif // MATRIX_H
