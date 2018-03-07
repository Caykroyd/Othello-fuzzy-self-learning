#include "Matrix.h"
#include <iostream>
using namespace std;

Matrix::Matrix()
{
//    cell = new int[B_SIZE][B_SIZE];
}

Matrix::~Matrix()
{
    //dtor
}

Matrix::Matrix(const Matrix& other)
{
    for(int i=0; i<B_SIZE; i++)
        for(int j=0; j<B_SIZE; j++)
            cell[i][j] = other.cell[i][j];
}

void Matrix::Reset()
{
	for (int i = 0; i < B_SIZE; i++)
		for (int j = 0; j < B_SIZE; j++)
			cell[i][j] = 0;
}

void Matrix::Print(string zero, string pos, string neg)
{

    cout << endl << "   ";
    for (int i = 0; i < B_SIZE; i++)
        cout << i+1 << "  ";
    cout << endl;

	for (int j = 0; j < B_SIZE; j++) {

        cout << j+1 << " ";
		for (int i = 0; i < B_SIZE; i++) {
            if (cell[i][j] == 1)
                cout <<  " " << pos << " ";
            else if (cell[i][j] == -1)
                cout <<  " " << neg << " ";
            else if (cell[i][j] == 0)
                cout <<  " " << zero << " ";
            else
                cout << " E ";
		}
        cout << endl;
    }
    cout << endl;
}
