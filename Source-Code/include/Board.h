#ifndef BOARD_H
#define BOARD_H

#define B_SIZE 6
#define B_CENTRE B_SIZE/2
#define SYM_POS B_CENTRE*(B_CENTRE+1)/2

#include <list>
#include "Position.h"
#include "Matrix.h"
#include "Features.h"
using namespace std;


class Features;

class Board
{
	public:
		Board();
		Board(const Board &original);
		~Board();

		void Init();

		void FindMoves();
		int GetPlayer();
		void Play(Position p);
		void Pass();

        Features* featuresBlack;
        Features* featuresWhite;

        Matrix* config;
		Matrix* moves;
		Matrix* stability;

		bool stableflag;

		list<Position>* MatrixToList(Matrix* m);

		float value;// minimax value

		int ply;
	private:
		int currentPlayer;
		void FlipLine(int i, int j, int a, int b);
		bool CheckPos(int i, int j);
		bool CheckLine(int i, int j, int a, int b);

        bool IsEdge(int i, int j);
        bool LineIsFull(int origin_x, int origin_y, int a, int b);

        void CalculateStability(int player);
        void RecursiveStabilityCheck(int x, int y, int player, bool H[B_SIZE], bool V[B_SIZE], bool SH[B_SIZE], bool SV[B_SIZE], bool DH[B_SIZE], bool DV[B_SIZE]);
        bool IsStable(int i, int j, bool H[B_SIZE], bool V[B_SIZE], bool SH[B_SIZE], bool SV[B_SIZE], bool DH[B_SIZE], bool DV[B_SIZE]);
        bool IsDirectionallyStable(int x, int y, int a, int b, bool fullLine);
        bool StableFlag(int i, int j);
};

#endif // BOARD_H
