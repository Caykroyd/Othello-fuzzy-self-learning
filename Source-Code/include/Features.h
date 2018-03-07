#ifndef FEATURES_H
#define FEATURES_H
#include "Board.h"

class Board;

class Features
{
    public:
        Features();
        Features(const Features &original);
        ~Features();

        int advantage;
        int stability;
        int mobility;
        int xmobility;
        int potmobilityA;
        int potmobilityB;
        int xpotmobilityA;
        int xpotmobilityB;

        int posatr[SYM_POS];

        void Calculate(Board *b, int player);
        static void PreCalculate(Features *f1, Features *f2);
        void Print();
    protected:
        bool CheckAdj(Board *b, int i, int j, int value);
        bool CheckPos(Board *b, int i, int j, int value);
        int SumSymmetricalPos(Board *b, int i, int j);

        void CalculateStability(Board *b);
    private:
};

#endif // FEATURES_H
