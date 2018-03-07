#define DEPTH 5

#include <iostream>
#include "time.h"
#include "Board.h"
#include "Thinker.h"
#include "Generation.h"
#include "Persistence.h"

using namespace std;

string Player(int p)
{
    if(p == 1)
        return "+";
    else
        return "*";
}

void Display(Board &b)
{
        b.config->Print("-","+","*");

        cout << "Player " << Player(b.GetPlayer()) << " to play. " << endl;


        //cout << "Calculated Features: " << endl;
        //b->featuresBlack->Print();
        //b->featuresWhite->Print();
}

bool HumanPlay(Board &b)
{
    Position p;
    int i=0;
    int j=0;
    cout << "Human's turn. Available moves:" << endl;
    b.FindMoves();
    b.moves->Print("-","O","E");

    list<Position> *l = b.MatrixToList(b.moves);
    if(l->empty()) {
        b.Pass();
        cout << "Human has passed since there are no available moves." << endl;
        delete l;
        return true;
    }
    cin >> j; p.j = j-1;
    cin >> i; p.i = i-1;
    while(b.moves->cell[p.i][p.j] == 0) {
        cout << "Invalid Move! Try again..." << endl;
        cin >> j; p.j = j-1;
        cin >> i; p.i = i-1;
    }
    b.Play(p);
    delete l;
    return false;
}

void MvPMatch(Thinker &t, int team)
{
    t.Set(team);

    cout << "Match against Machine " << t.name << "." << endl;
    cout << "Player " << Player(1) << " (Black) starts." << endl;
    Board b;
    b.Init();
    bool pass = false;

    if(team==1) {
        Display(b);
        t.Play(b);
    }

    while(!pass){
        Display(b);
        pass = HumanPlay(b);

        Display(b);
        pass = t.Play(b) && pass;
    }

    cout << "The game has ended: ";

    int blackAdvantage = b.featuresBlack->advantage;

    if(blackAdvantage > 0)
        cout << "Player " << Player(1) << " wins! (" << blackAdvantage << ")" << endl;
    else if(blackAdvantage < 0)
        cout << "Player " << Player(-1) << " wins!(" << -blackAdvantage << ")" << endl;
    else if(blackAdvantage == 0)
        cout << "Draw!" << endl;
}

void PvPMatch()
{

    cout << "Match against Player." << endl;
    cout << "Player " << Player(1) << " (Black) starts." << endl;
    Board b;
    b.Init();
    bool pass = false;

    while(!pass){
        Display(b);
        pass = HumanPlay(b);

        Display(b);
        pass = HumanPlay(b) && pass;
    }

    cout << "The game has ended: ";

    int blackAdvantage = b.featuresBlack->advantage;

    if(blackAdvantage > 0)
        cout << "Player " << Player(1) << " wins! (" << blackAdvantage << ")" << endl;
    else if(blackAdvantage < 0)
        cout << "Player " << Player(-1) << " wins!(" << -blackAdvantage << ")" << endl;
    else if(blackAdvantage == 0)
        cout << "Draw!" << endl;
}

int main()
{
    srand(time(NULL));

    cout << "Welcome to Othello." << endl;

    Persistence persist("log.txt");

    cout << "Would you like to load the previously saved population? ( y / n )" << endl;

    char c;
    cin >> c;
    while(c!='y' && c!='n') {
        cout << "Invalid value. Try again." << endl;
        cin >> c;
    }

    Generation pop;

    if(c == 'y') {

        if(!persist.Retrieve(pop)) {
            cout << "Cannot retrieve file. Population will be randomized." << endl;
            pop.Randomize();
        }
    }
    else {
        pop.Randomize();
    }

    cout << "Please type: " << endl;
    cout << " - '1' to play against a Machine" << endl;
    cout << " - '2' for a Machine Tournament" << endl;
    cout << " - '3' for Machine Training" << endl;

    int in;
    cin >> in;
    while(in!=1 && in!=2 && in!=3) {
        cout << "Invalid value. Try again." << endl;
        cin >> in;
    }

    if( in == 1) {
        cout << "Pick your team (" << Player(1) << ": 1 or " << Player(-1) << ": -1):" << endl;

        cin >> in;
        while(in!=1 && in!=-1) {
            cout << "Invalid value. Try again." << endl;
            cin >> in;
        }

        Thinker* t = pop.GetTrainer(0);

        if(t == nullptr)
            t = pop.GetPlayer(0);

        MvPMatch(*t,-in);

        cout << "Match results for human battles are NOT saved." << endl;
    }
    else if( in == 2) {

        pop.Tournament();

        cout << "Generation is evolving based on the results of the tournament..." << endl;

        pop.Evolve();

        cout << "Tournament results are being saved..." << endl;

        persist.Save(pop);

        cout << "Population saved!" << endl;

    }
    else if( in == 3) {

        cout << "How many epochs?" << endl;
        unsigned int ui;
        cin >> ui;

        cout << "Generating a reference player..." << endl;
        Thinker referee;

        float avgScore[ui];
        int minScore[ui], maxScore[ui];

        for(int k = 0; k < ui; k++) {
            cout << "Reference match " << k << ":" << endl;
            avgScore[k] = pop.ReferenceMatch(referee, PEOPLE, minScore[k], maxScore[k]);
            cout << endl << "The players scored an average of " << avgScore[k] << " out of " << B_SIZE * B_SIZE;
            cout << " within a margin of [" << minScore[k] << ", " << maxScore[k] << "], against the referee." << endl;

            cout << "Training session " << k << ":" << endl;

            pop.Training();

            cout << "Population is evolving based on the results of the training..." << endl;

            pop.Evolve();

        }

        cout << "Training results are being saved..." << endl;

        persist.Save(pop);

        cout << "Population saved!" << endl;

        persist.SaveGraph(avgScore,ui,"avgData.txt","average");
        persist.SaveGraph(maxScore,ui,"maxData.txt","maximum");
        persist.SaveGraph(minScore,ui,"minData.txt","minimum");
    }
    cout << "Type anything to abort..." << endl;
    cin >> c;

    return 0;
}
