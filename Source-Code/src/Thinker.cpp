#include "Thinker.h"

string Player(int p);

int Thinker::id = 0;

Thinker::Thinker()
{
    id++;
    name = id;

    score = 0;

    uint8_t parameter[PARAMETERS];
    for(int i = 0; i < PARAMETERS; i++)
        parameter[i] = rand()%255;

    Decode(parameter);
}

Thinker::Thinker(int parameter[PARAMETERS])
{
    id++;
    name = id;

    score = 0;

    int i = 0;
    wAdvantage = parameter[i]; i++;
    wStability = parameter[i]; i++;
    wMobility = parameter[i]; i++;
    wXmobility = parameter[i]; i++;
    wPotmobilityA = parameter[i]; i++;
    wPotmobilityB = parameter[i]; i++;
    wXpotmobilityA = parameter[i]; i++;
    wXpotmobilityB = parameter[i]; i++;

    for(int index=0; index<SYM_POS; index++) {
        wPosatr[index] = parameter[i]; i++;
    }

    for(int index=0; index < 4*WEIGHTS; index++){
        MFVar[index] = parameter[i]; i++;
    }
}

Thinker::Thinker(uint8_t parameter[PARAMETERS])
{
    id++;
    name = id;

    score = 0;

    Decode(parameter);
}

Thinker::Thinker(const Thinker& original)
{
    id++;
    name = id;

    score = original.score;

    wAdvantage = original.wAdvantage;
    wStability = original.wStability;
    wMobility = original.wMobility;
    wXmobility = original.wXmobility;
    wPotmobilityA = original.wPotmobilityA;
    wPotmobilityB = original.wPotmobilityB;
    wXpotmobilityA = original.wXpotmobilityA;
    wXpotmobilityB = original.wXpotmobilityB;

    for(int index=0; index<SYM_POS; index++)
        wPosatr[index] = original.wPosatr[index];

    for(int i=0; i < WEIGHTS; i++) {
        MFVar[4*i+0] = original.MFVar[4*i+0]; // Pos
        MFVar[4*i+1] = original.MFVar[4*i+1]; // HWidth
        MFVar[4*i+2] = original.MFVar[4*i+2]; // LSlope
        MFVar[4*i+3] = original.MFVar[4*i+3]; // RSlope
    }
}

void Thinker::Set(int colour)
{
    this->colour = colour;
}

Thinker::~Thinker()
{
    //dtor
}

bool Thinker::Play(Board &b)
{
    bool pass;

    Position p;

    Minimax(b,p,pass,DEPTH);

    if(pass) {
        //cout << "Ply " << b.ply << ": Machine " << name << " has passed its turn." << endl;
        b.Pass();
    }
    else {
        //cout << "Ply " << b.ply << ": Machine " << name << " has played (" << p.i + 1 << "," << p.j + 1 << ") " << "as " << Player(colour) << endl;
        b.Play(p);
    }
    return pass;
}

void Thinker::PrintWeights()
{

    cout << "WEIGHTS: advantage: " << wAdvantage << endl << "mobility: " << wMobility << endl << "xmobility: " << wXmobility << endl << "potmobilityA: " << wPotmobilityA << endl << "potmobilityB: " << wPotmobilityB << endl;

    for(int index=0; index<SYM_POS; index++ )
        cout << "wPosatr[" << index << "] = " << wPosatr[index] << ", ";

    for(int index=0; index<4*(WEIGHTS); index++ )
        cout << "MFVar[" << index << "] = " << MFVar[index] << ", ";

    cout << endl;
}

string Thinker::WeightsToString()
{
    ostringstream out;

    // Not printing name or score!

    uint8_t parameters[PARAMETERS];
    Encode(parameters);

    for(int index=0; index<PARAMETERS; index++) {
            out << (int)parameters[index] << endl;
    }

    string str = out.str();

    return str;
}

float Thinker::Minimax(Board &b, Position &bestMove, bool &pass, int depth)
{
    b.FindMoves();
    //b->moves->Print("-","O","E");
    list<Position>* moves = b.MatrixToList(b.moves);

    if(moves->empty()) {
        pass = true;
        delete moves;
        int blackAdvantage = b.featuresBlack->advantage;

        if(blackAdvantage == 0)
            return 0;
        else if(blackAdvantage * colour > 0)
            return INF;
        else
            return - INF;
    }

    float bestValue, value;

    if(colour == b.GetPlayer())
        bestValue = - INF;
    else
        bestValue = INF;
    while(!moves->empty()){
        Board board(b);
        Position position = moves->back();
        board.Play(position);

        value = Minimax(board, depth - 1);

        if(colour == b.GetPlayer() && value >= bestValue) {
            bestValue = value;
            bestMove.i = position.i;
            bestMove.j = position.j;

        }
        else if(colour != b.GetPlayer() && value <= bestValue){
            cerr << "Wrong player! " << colour << " != " << b.GetPlayer() << endl;
        }

         moves->pop_back(); // pop_back deletes the element
    }
    delete moves;

    pass = false;
    return bestValue;
}

float Thinker::Minimax(Board &b, int depth)
{
    b.FindMoves();
    list<Position>* moves = b.MatrixToList(b.moves);

    if(moves->empty()){
        delete moves;

        int blackAdvantage = b.featuresBlack->advantage;
        if(blackAdvantage == 0)
            return 0;
        else if(blackAdvantage * colour > 0)
            return INF;
        else
            return - INF;
    }
    else if(depth == 0) {
        delete moves;
        return HeuristicValue(b);
    }

    float bestValue, value;

    if(colour == b.GetPlayer())
        bestValue = - INF;
    else
        bestValue = + INF;

    while(!moves->empty()){
        Board board(b);
        Position position = moves->back(); moves->pop_back(); // pop_back deletes the element
        board.Play(position);

        value = Minimax(board, depth - 1);

        if(colour == b.GetPlayer() && value > bestValue) {
            bestValue = value;
        }
        else if(colour != b.GetPlayer() && value < bestValue){
            bestValue = value;
        }
    }

    delete moves;

    return bestValue;
}

float Thinker::HeuristicValue(Board &b)
{
    float value = 0;
    Features* f;
    if(colour == 1)
        f = b.featuresBlack;
    else if(colour == -1)
        f = b.featuresWhite;
    else
        throw 0;

    value += f->advantage * wAdvantage * MembershipFunction(b.ply, MFVar[0], MFVar[0+1], MFVar[0+2], MFVar[0+3]);
    value += f->stability * wStability * MembershipFunction(b.ply, MFVar[4], MFVar[4+1], MFVar[4+2], MFVar[4+3]);
    value += f->mobility * wMobility * MembershipFunction(b.ply, MFVar[8], MFVar[8+1], MFVar[8+2], MFVar[8+3]);
    value += f->xmobility * wXmobility * MembershipFunction(b.ply, MFVar[12], MFVar[12+1], MFVar[12+2], MFVar[12+3]);
    value += f->potmobilityA * wPotmobilityA * MembershipFunction(b.ply, MFVar[16], MFVar[16+1], MFVar[16+2], MFVar[16+3]);
    value += f->potmobilityB * wPotmobilityB * MembershipFunction(b.ply, MFVar[20], MFVar[20+1], MFVar[20+2], MFVar[20+3]);
    value += f->xpotmobilityB * wXpotmobilityB * MembershipFunction(b.ply, MFVar[24], MFVar[24+1], MFVar[24+2], MFVar[24+3]);
    value += f->xpotmobilityB * wXpotmobilityB * MembershipFunction(b.ply, MFVar[28], MFVar[28+1], MFVar[28+2], MFVar[28+3]);

    for(int index=0; index<SYM_POS; index++)
        value += f->posatr[index] * wPosatr[index] * MembershipFunction(b.ply, MFVar[32+4*index], MFVar[32+4*index+1], MFVar[32+4*index+2], MFVar[32+4*index+3]);

    return value;
}

float Thinker::MembershipFunction(int ply, float pos, float hwidth, float lslope, float rslope)
{
/// The membership functions used here are all trapezoidal.
/// 0 < pos < 60, step 0.1
/// 0 < hwidth < 60, step 0.1
/// 0 < rslope, lslope < 0.1, step 0.001

    int val;

    if(ply <= pos + hwidth && ply >= pos - hwidth)
        return 1;

    else if(ply > pos + hwidth)
        val = 1 - (ply - pos - hwidth) * rslope;

    else if(ply < pos - hwidth)
        val = ply * lslope;

    if(val < 0)
        return 0;
    else if(val > 1)
        return 1;
    else
        return val;
}

void Thinker::Encode(uint8_t parameter[PARAMETERS])
{
    int i = 0;

    // 0 < w < 10
    parameter[i] = round(wAdvantage * 25.5f); i++;
    parameter[i] = round(wStability * 25.5f); i++;
    parameter[i] = round(wMobility * 25.5f); i++;
    parameter[i] = round(wPotmobilityA * 25.5f); i++;
    parameter[i] = round(wPotmobilityB * 25.5f); i++;
    parameter[i] = round(- wXmobility * 25.5f); i++; // Negative
    parameter[i] = round(- wXpotmobilityA * 25.5f); i++; // Negative
    parameter[i] = round(- wXpotmobilityB * 25.5f); i++; // Negative

    // -10 / SYM_POS < w < 10 / SYM_POS
    for(int index=0; index<SYM_POS; index++) {
        parameter[i] = round((wPosatr[index] * SYM_POS + 10) * ( 12.75f )); i++;
    }

    for(int index=0; index < WEIGHTS; index++){
        // 0 < pos < 60
        parameter[i] = round(MFVar[4 * index + 0] * 4.25f); i++;
        // 0 < hwidth < 60
        parameter[i] = round(MFVar[4 * index + 1] * 4.25f); i++;
        // 0 < lslope < 0.1
        parameter[i] = round(MFVar[4 * index + 2] * 2550.0f); i++;
        // 0 > rslope > -0.1
        parameter[i] = round((- MFVar[4 * index + 3]) * 2550.0f); i++;
    }
}

void Thinker::Decode(uint8_t parameter[PARAMETERS])
{
    int i = 0;

    // 0 < w < 10
    wAdvantage = (int)parameter[i] / 25.5f; i++;
    wStability = (int)parameter[i] / 25.5f; i++;
    wMobility = (int)parameter[i] / 25.5f; i++;
    wPotmobilityA = (int)parameter[i] / 25.5f; i++;
    wPotmobilityB = (int)parameter[i] / 25.5f; i++;
    wXmobility = - (int)parameter[i] / 25.5f; i++; // Negative
    wXpotmobilityA = - (int)parameter[i] / 25.5f; i++; // Negative
    wXpotmobilityB = - (int)parameter[i] / 25.5f; i++; // Negative

    // -10 < w < 10 / SYM_POS
    for(int index=0; index<SYM_POS; index++) {
        wPosatr[index] = ( (int)parameter[i] / 12.75f - 10 ) / SYM_POS; i++;
    }

    for(int index=0; index < WEIGHTS; index++){
        // 0 < pos < 60
        MFVar[4 * index + 0] = (int)parameter[i] / 4.25f; i++;
        // 0 < hwidth < 60
        MFVar[4 * index + 1] = (int)parameter[i] / 4.25f; i++;
        // 0 < lslope < 0.1
        MFVar[4 * index + 2] = (int)parameter[i] / 2550.0f; i++;
        // 0 > rslope > -0.1
        MFVar[4 * index + 3] = - (int)parameter[i] / 2550.0f; i++;
    }

}
