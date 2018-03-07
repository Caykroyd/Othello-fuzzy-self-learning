#include "Features.h"
#include <iostream>

Features::Features()
{
    advantage = 0;
    mobility = 0;
    potmobilityA = 0;
    potmobilityB = 0;
    xmobility = 0;
    xpotmobilityA = 0;
    xpotmobilityB = 0;
    stability = 0;

    for(int i=0; i<B_CENTRE; i++ ){
        for(int j=0; j<=i; j++){
            int index = i*(i+1)/2 + j;
            posatr[index] = 0;
        }
    }
}

Features::Features(const Features &original)
{
    advantage = original.advantage;
    mobility = original.mobility;
    xmobility = original.xmobility;
    potmobilityA = original.potmobilityA;
    potmobilityB = original.potmobilityB;
    xpotmobilityA = original.xpotmobilityA;
    xpotmobilityB = original.xpotmobilityB;
    stability = original.stability;

    for(int i=0; i<B_CENTRE; i++ ){
        for(int j=0; j<=i; j++){
            int index = i*(i+1)/2 + j;
            posatr[index] = original.posatr[index];
        }
    }
}

Features::~Features()
{

}


void Features::PreCalculate(Features *f1, Features *f2)
{
    f1->xmobility = f2->mobility;
    f2->xmobility = f1->mobility;

    f1->xpotmobilityA = f2->potmobilityA;
    f2->xpotmobilityA = f1->potmobilityA;

    f1->xpotmobilityB = f2->potmobilityB;
    f2->xpotmobilityB = f1->potmobilityB;
}

void Features::Calculate(Board *b, int player)
{

    // Basic Features
    advantage = 0;
    mobility = 0;
    stability = 0;
    potmobilityA = 0;
    potmobilityB = 0;
    b->FindMoves();

    // Majority of features
    for(int i=0; i<B_SIZE; i++ ){
        for(int j=0; j<B_SIZE; j++){
            mobility += b->moves->cell[i][j];
            advantage += b->config->cell[i][j] * player;
            if(b->config->cell[i][j] == -player && CheckAdj(b,i,j,0))
                potmobilityA++;
            if(b->config->cell[i][j] == 0 && CheckAdj(b,i,j,-player))
                potmobilityB++;
            stability += b->stability->cell[i][j];
        }
    }

    // Configurational Features
    for(int i=0; i<B_CENTRE; i++ ){
        for(int j=0; j<=i; j++){
            int index = i*(i+1)/2 + j;
            posatr[index] = SumSymmetricalPos(b,i,j);
        }
    }

}

void Features::Print()
{
    cout << "advantage: " << advantage << " " << "mobility: " << mobility << " " << "xmobility: " << xmobility << " " << "potmobilityA: " << potmobilityA << " " << "potmobilityB: " << potmobilityB << endl;

    for(int index=0; index<SYM_POS; index++ )
        cout << "posatr[" << index << "] = " << posatr[index] << endl;
}

bool Features::CheckAdj(Board *b, int i, int j, int value)
{
    if(CheckPos(b,i+1,j,value) || CheckPos(b,i+1,j+1,value) || CheckPos(b,i,j+1,value) || CheckPos(b,i-1,j+1,value) || CheckPos(b,i-1,j,value) || CheckPos(b,i-1,j-1,value) || CheckPos(b,i,j-1,value) || CheckPos(b,i+1,j-1,value))
        return true;
    else
        return false;
}

int Features::SumSymmetricalPos(Board *b, int i, int j)
{
    int sum = 0;
    int edge = B_SIZE - 1;
    sum += b->config->cell[i][j];
    sum += b->config->cell[edge-i][j];
    sum += b->config->cell[i][edge-j];
    sum += b->config->cell[edge-i][edge-j];

    // Dont sum if on diagonal!!!!
    if(i != j) {
        sum += b->config->cell[j][i];
        sum += b->config->cell[edge-j][i];
        sum += b->config->cell[j][edge-i];
        sum += b->config->cell[edge-j][edge-i];
    }

    return sum;
}

bool Features::CheckPos(Board *b, int i, int j, int value)
{
    return (i >= 0 && i < B_SIZE && j >= 0 && j < B_SIZE && b->config->cell[i][j] == value);
}
