#ifndef THINKER_H
#define THINKER_H


#include "Board.h"
#include "Features.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <tgmath.h>
#define INF 100000

#define DEPTH 3
#define WEIGHTS 8+SYM_POS
#define PARAMETERS (4+1)*WEIGHTS + 50

using namespace std;

class Thinker
{
    public:
        Thinker(); // Randomly generates a player
        Thinker(int parameter[PARAMETERS]); // Generates a player from an array of parameters (in the format WeightsToString).
        Thinker(uint8_t parameter[PARAMETERS]); // Generates a player from an array of parameters (in the format WeightsToString).
        Thinker(const Thinker& original);
        ~Thinker();

        void Set(int colour);

        bool Play(Board &b);

        int name;
        int score;
        int colour;

        void PrintWeights();
        string WeightsToString();

        void Encode(uint8_t parameter[PARAMETERS]);
        void Decode(uint8_t parameter[PARAMETERS]);
    protected:
        float wAdvantage;
        float wStability;
        float wMobility;
        float wXmobility;
        float wPotmobilityA;
        float wPotmobilityB;
        float wXpotmobilityA;
        float wXpotmobilityB;

        float wPosatr[SYM_POS];

        float MFVar[4*(WEIGHTS)];

        float Minimax(Board &b, Position &bestMove, bool &pass, int depth); // This is what must be called by Play().
        float Minimax(Board &b, int depth);
        float HeuristicValue(Board &b);
        float MembershipFunction(int ply, float pos, float hwidth, float lslope, float rslope);

        static int id;

    private:
};

#endif // THINKER_H
