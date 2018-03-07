#include "Board.h"
#include <iostream>
using namespace std;

Board::Board()
{
    config = new Matrix();
    config->Reset();

    currentPlayer = 1; // black

    moves = new Matrix();

    stability = new Matrix();
    stability->Reset();
    stableflag = false;

    featuresBlack = new Features();
    featuresWhite = new Features();

    ply = 0;
}

Board::~Board()
{
    delete config;
    delete featuresBlack;
    delete featuresWhite;
    delete moves;
    delete stability;
}

Board::Board(const Board &original)
{
	config = new Matrix();

	for (int i = 0; i < B_SIZE; i++)
		for (int j = 0; j < B_SIZE; j++)
			config->cell[i][j] = original.config->cell[i][j];

	currentPlayer = original.currentPlayer;

    moves = new Matrix(*original.moves);

    stability = new Matrix(*original.stability);
    stableflag = original.stableflag;

    featuresBlack = new Features(*original.featuresBlack);
    featuresWhite = new Features(*original.featuresWhite);

    ply = original.ply;
}

void Board::Init()
{
    int half_p = B_SIZE/2;
    int half_m = half_p-1;

    config->cell[half_p][half_p] = 1;
    config->cell[half_p][half_m] = -1;
    config->cell[half_m][half_p] = -1;
    config->cell[half_m][half_m] = 1;

	FindMoves();
	featuresBlack->Calculate(this,1);
	featuresWhite->Calculate(this,-1);
    featuresBlack->xmobility = featuresWhite->mobility;
    featuresWhite->xmobility = featuresBlack->mobility;
}

void Board::FindMoves()
{
    for(int i = 0; i<B_SIZE; i++)
        for(int j = 0; j<B_SIZE; j++)
            if(CheckPos(i,j))
                moves->cell[i][j] = 1;
            else
                moves->cell[i][j] = 0;

}

list<Position> *Board::MatrixToList(Matrix *m)
{
    list<Position> *l = new list<Position>();

    for(int i=0; i<B_SIZE; i++)
        for(int j=0; j<B_SIZE; j++)
            if(m->cell[i][j] == 1) {
                Position p(i,j);
                l->push_back(p);
            }

    return l;
}

int Board::GetPlayer()
{
	return currentPlayer;
}

void Board::Play(Position p)
{
    int i = p.i;
    int j = p.j;

    if(config->cell[i][j] != 0) {
        cerr << "Invalid move!" << endl;
        throw "Invalid Move!";
        return;
    }

    Features::PreCalculate(featuresBlack, featuresWhite);

    config->cell[i][j] = currentPlayer;

    // Now we must capture enemy pieces in adjacent lines.
    FlipLine(i,j,0,1);
    FlipLine(i,j,0,-1);
    FlipLine(i,j,1,0);
    FlipLine(i,j,-1,0);
    FlipLine(i,j,1,1);
    FlipLine(i,j,1,-1);
    FlipLine(i,j,-1,1);
    FlipLine(i,j,-1,-1);

	currentPlayer = - currentPlayer;

    ply++;

	FindMoves();
	if(StableFlag(i,j))
        stableflag = true;
    CalculateStability(currentPlayer);
	featuresBlack->Calculate(this,1);
	featuresWhite->Calculate(this,-1);
}

void Board::Pass()
{
    Features::PreCalculate(featuresBlack, featuresWhite);

	currentPlayer = - currentPlayer;

    ply++;

	FindMoves();
    CalculateStability(currentPlayer);
	featuresBlack->Calculate(this,1);
	featuresWhite->Calculate(this,-1);
}

void Board::FlipLine(int i, int j, int a, int b)
{
    int k = 0;

    // Propagate Forward to check how many we can capture
    do
        k++;
    while(i + a*k < B_SIZE && i + a*k >= 0 && j + b*k < B_SIZE && j + b*k >= 0 && config->cell[i+a*k][j+b*k] == - currentPlayer);

    // Propagate backwards and flip all pieces on the way
    if(k>1 && i + a*k < B_SIZE && i + a*k >= 0 && j + b*k < B_SIZE && j + b*k >= 0 && config->cell[i+a*k][j+b*k] == currentPlayer) {
        for(k--; k > 0; k--) {
            config->cell[i+a*k][j+b*k] = currentPlayer;
        }
    }
}

bool Board::CheckPos(int i, int j)
{
    if(config->cell[i][j]!=0)
        return false;
    else if(CheckLine(i,j,1,0))
        return true;
    else if(CheckLine(i,j,0,1))
        return true;
    else if(CheckLine(i,j,-1,0))
        return true;
    else if(CheckLine(i,j,0,-1))
        return true;
    else if(CheckLine(i,j,1,1))
        return true;
    else if(CheckLine(i,j,1,-1))
        return true;
    else if(CheckLine(i,j,-1,1))
        return true;
    else if(CheckLine(i,j,-1,-1))
        return true;

    return false;
}

bool Board::CheckLine(int i, int j, int a, int b)
{
    int k = 1;

    // Propagate Forward to check how many we can capture
    while(i + a*k < B_SIZE && i + a*k >= 0 && j + b*k < B_SIZE && j + b*k >= 0 && config->cell[i+a*k][j+b*k] == - currentPlayer)
        k++;

    // Check if our piece is at the end of the sandwitch
    return k>1 && i + a*k < B_SIZE && i + a*k >= 0 && j + b*k < B_SIZE && j + b*k >= 0 && config->cell[i+a*k][j+b*k] == currentPlayer;
}

bool Board::IsEdge(int i, int j)
{
    return i >= B_SIZE || i < 0 || j >= B_SIZE || j < 0;
}

bool Board::StableFlag(int i, int j)
{
	if((i == 0 || i == B_SIZE-1) && (j <= 1 || j >= B_SIZE-2))
        return true;

	if((j == 0 || j == B_SIZE-1) && (i <= 1 || i >= B_SIZE-2))
        return true;

    return false;
}

void Board::CalculateStability(int player)
{
    stability->Reset();

    // If the 12 corner positions are empty, we do not need to try and calculate.
    if(stableflag == false)
        return;

    // Initiating directional stability vectors: Horizontal, Vertical, Sinister (top-left), Dexter (top-right);
    // Note that SH, SV or DH, DV overlap at the corners.
    bool H[B_SIZE], V[B_SIZE], SH[B_SIZE], SV[B_SIZE], DH[B_SIZE], DV[B_SIZE];

    // Phase 1. Full Lines (we are ignoring the case of semi-full lines, which can bring partial stability)
    for(int i = 0; i < B_SIZE; i++) {
        H[i] = false;
        V[i] = false;
        SH[i] = false;
        SV[i] = false;
        DH[i] = false;
        DV[i] = false;
        if(LineIsFull(0,i,1,0))
            H[i] = true;
        if(LineIsFull(i,0,0,1))
            V[i] = true;
        if(LineIsFull(0,i,1,1))
            SH[i] = true;
        if(LineIsFull(i,0,1,1))
            SV[i] = true;
        if(LineIsFull(B_SIZE-1,i,-1,1))
            DH[i] = true;
        if(LineIsFull(B_SIZE-1-i,0,-1,1))
            DV[i] = true;
    }

    // Phase 2. Recursive Analysis: beginning at full lines.
    for(int i = 0; i < B_SIZE; i++) {
        if(H[i] == true) { // With this check we reduce the computational time
            for(int j = 0; j < B_SIZE; j++) {
                if(config->cell[i][j] == player && IsStable(i,j,H,V,SH,SV,DH,DV)) {
                    RecursiveStabilityCheck(i,j,player,H,V,SH,SV,DH,DV);
                }
            }
        }
    }

    // Phase 3. Recursive Analysis: beginning at corners.
    RecursiveStabilityCheck(0,0,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(0,B_SIZE-1,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(B_SIZE-1,0,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(B_SIZE-1,B_SIZE-1,player,H,V,SH,SV,DH,DV);
}

void Board::RecursiveStabilityCheck(int x, int y, int player, bool H[B_SIZE], bool V[B_SIZE], bool SH[B_SIZE], bool SV[B_SIZE], bool DH[B_SIZE], bool DV[B_SIZE])
{
    // If this position is not on the board, ignore it.
    if(IsEdge(x,y))
        return;

    // If we've already checked, ignore.
    if(stability->cell[x][y] == 1)
        return;

    // If this piece is not stable, ignore.
    if(config->cell[x][y] != player || !IsStable(x,y,H,V,SH,SV,DH,DV))
        return;

    // We can safely conclude this is a valid, stable position.
    stability->cell[x][y] = 1;

    RecursiveStabilityCheck(x,y+1,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(x,y-1,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(x+1,y,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(x-1,y,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(x+1,y+1,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(x+1,y-1,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(x-1,y+1,player,H,V,SH,SV,DH,DV);
    RecursiveStabilityCheck(x-1,y-1,player,H,V,SH,SV,DH,DV);

}

bool Board::LineIsFull(int origin_x, int origin_y, int a, int b)
{
    for(int k=0; IsEdge( origin_x + a * k, origin_y + b * k ) == false; k++){

        if(config->cell[ origin_x + a * k ][ origin_y + b * k ] == 0)
            return false;

    }

    return true;
}

bool Board::IsStable(int i, int j, bool H[B_SIZE], bool V[B_SIZE], bool SH[B_SIZE], bool SV[B_SIZE], bool DH[B_SIZE], bool DV[B_SIZE])
{
    if(!IsDirectionallyStable(i,j,1,0,H[j]))
        return false;

    if(!IsDirectionallyStable(i,j,0,1,V[i]))
        return false;

    bool fullLine = i > j ? SV[i-j] : SH[j-i];
    if(!IsDirectionallyStable(i,j,1,1,fullLine))
        return false;

    bool index = (B_SIZE - 1) - (i + j);
    fullLine = index > 0 ? DV[index] : DH[-index];
    if(!IsDirectionallyStable(B_SIZE-1,i,-1,1,fullLine))
        return false;

    return true;

}

bool Board::IsDirectionallyStable(int x, int y, int a, int b, bool fullLine)
{
    /// (x,y) is the origin of the piece.
    /// a and b must be either 0, 1 or -1, with at least one non-zero.

    if(fullLine)
        return true;
    else if(IsEdge( x + a, y + b ) || IsEdge( x - a, y - b ))
        return true;
    else if(stability->cell[x+a][y+b] == 1 || stability->cell[x-a][y-b] == 1)
        return true;
    else
        return false;
}
