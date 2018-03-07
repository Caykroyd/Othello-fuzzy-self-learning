#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "iostream"
#include "fstream"
#include "stdexcept"

#include "Generation.h"
#include "Thinker.h"
#include "Board.h" // constants

using namespace std;

class Persistence
{
    public:
        Persistence(string file);
        ~Persistence();

        bool Save(Generation &generation);
        bool Retrieve(Generation &generation);
        bool SaveGraph(float data[], int length, string fname, string title);
        bool SaveGraph(int data[], int length, string fname, string title);
    protected:
        string file;
        string graph;

        bool SavePlayer(ofstream& output, Thinker* player);
        Thinker* RetrievePlayer(ifstream& input);

    private:
};

#endif // PERSISTENCE_H
