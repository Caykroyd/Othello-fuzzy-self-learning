#include "Generation.h"

float Generation::MutationChance = 20; // in (%)
float Generation::CrossoverChance = 100; // in (%)

Generation::Generation()
{
    for(int i=0; i<PEOPLE; i++)
        person[i] = nullptr;

    for(int i=0; i<TRAINERS; i++)
        trainer[i] = nullptr;

    people = 0;
    trainers = 0;
}

Generation::~Generation()
{
    Reset();
}

void Generation::Reset()
{
    for(int i = 0; i < people; i++) {
        delete person[i];
        person[i] = nullptr;
    }

    for(int i=0; i<TRAINERS; i++) {
        delete trainer[i];
        trainer[i] = nullptr;
    }

    people = 0;
    trainers = 0;
}

void Generation::Copy(const Generation &original)
{
    Reset();

    for(int i = 0; i < original.people; i++) {
        Thinker* t = new Thinker(*original.person[i]);
        AddAsPerson(t);
    }

    for(int i=0; i<original.trainers; i++) {
        Thinker* t = new Thinker(*original.trainer[i]);
        AddAsTrainer(t);
    }
}

void Generation::Randomize()
{
    Reset();

    //srand(time(NULL));

    // Add Thinkers with random parameters...
    for(int i = 0; i<PEOPLE; i++)
        person[i] = new Thinker();

    people = PEOPLE;
}

void Generation::Tournament()
{
    for(int i=0; i<PEOPLE; i++)
        person[i]->score = 0;

    cout << "Progress (\\" << PEOPLE << "): ";
    int counter = 0;
    for(int i=0; i<PEOPLE; i++) {
        cout << i + 1 << " ";
        for(int j=i+1; j<PEOPLE; j++) {
            Match(person[i],person[j]);
            Match(person[j],person[i]);
        }
    }
    cout << endl << "End of Tournament." << endl;

    for(int i=0; i<PEOPLE; i++)
        if(person[i] != NULL)
            cout << "Machine " << person[i]->name << " has scored " << person[i]->score << "!" << endl;
}

void Generation::Training()
{
    if(trainers < TRAINERS){
        cerr << "Not enough trainers! " << trainers << "/" << TRAINERS << endl;
        return;
    }

    cout << "Progress (\\" << PEOPLE << "): ";
    for(int i=0; i<PEOPLE; i++) {
        cout << i + 1 << " ";
        person[i]->score = 0;
        for(int j=0; j<TRAINERS; j++) {
            Match(person[i],trainer[j]);
            Match(trainer[j],person[i]);
        }
    }
    cout << endl << "End of Training." << endl;
}

float Generation::ReferenceMatch(Thinker &t, int representatives, int &minScore, int &maxScore)
{
    int tscore = t.score;

    Thinker *rep[representatives];
    GetBest(rep, representatives);

    int score, originalScore;
    minScore = B_SIZE * B_SIZE; maxScore = 0;
    cout << "Progress (\\" << people << "): ";
    for(int i=0; i<representatives; i++) {
        cout << i + 1 << " ";
        originalScore = rep[i]->score;
        Match(rep[i],&t);
        Match(&t,rep[i]);

        score = (rep[i]->score - originalScore) / 2;
        if(maxScore < score)
            maxScore = score;
        if(minScore > score)
            minScore = score;
        rep[i]->score = originalScore;
    }
    float repAvg = (t.score - tscore) / (2.0f * people);

    return B_SIZE * B_SIZE - repAvg;
}

void Generation::AddAsPerson(Thinker* thinker)
{
    if(people >= PEOPLE) {
        cerr << "Too many people! (" << people << ")" << endl;
        return;
    }

    person[people] = thinker;
    people++;
}

void Generation::AddAsTrainer(Thinker* thinker)
{
    if(trainers >= TRAINERS) {
        cerr << "Too many trainers! (" << trainers << ")" << endl;
        return;
    }

    trainer[trainers] = thinker;
    trainers++;
}

Thinker* Generation::GetPlayer(int i)
{
    if(i >= people)
        return nullptr;

    return person[i];
}

Thinker* Generation::GetTrainer(int i)
{
    if(i >= trainers)
        return nullptr;

    return trainer[i];
}

void Generation::GetBest(Thinker* best[], int amount)
{
    if(amount > people)
        cerr << "Error in GetBest: There are too few people in the population!" << endl;

    // Add the first five people to the array
    for(int i=0; i<amount; i++){
        if(i < amount) {
            best[i] = person[i];
        }
    }

    int worst = 0; // the index of the worst person amongst the best
    for(int i=0; i<amount; i++)
        if(best[i]->score < best[worst]->score)
            worst = i;

    for(int i=amount; i<PEOPLE; i++){
        // If there's someone who's better than the 'worst of the best'
        if(person[i]->score > best[worst]->score) {
            // Substitute the previous worst for the new person
            best[worst] = person[i];
            // Find the new worst amongst them all
            for(int i=0; i<amount; i++)
                if(best[i]->score < best[worst]->score)
                    worst = i;
        }
    }
}

void Generation::Match(Thinker *black, Thinker *white)
{
    black->Set(1);
    white->Set(-1);

    Board b;
    b.Init();

    bool pass = false;

    while(!pass){
        pass = black->Play(b);

        pass = white->Play(b) && pass;
    }

    int blackAdvantage = b.featuresBlack->advantage;
    int blackCount = (blackAdvantage + B_SIZE*B_SIZE)/2;

    // The fitness function has to be positive, so instead of getting the difference in points, we get the absolute value.
    black->score += blackCount;
    white->score += B_SIZE*B_SIZE - blackCount;
}

void Generation::Evolve()
{
    Generation children;

    // Copy the 5 best players as trainers
    Thinker* best[TRAINERS];
    GetBest(best, TRAINERS);
    for(int i=0; i<TRAINERS; i++) {
        Thinker *t = new Thinker(*best[i]);
        children.AddAsTrainer(t);
    }

    // Select the players which will compose the next generation
    for(int i=0; i<PEOPLE/2; i++){

        uint8_t childA[PARAMETERS];
        uint8_t childB[PARAMETERS];

        // Pick the progenitors
        Select(childA);
        Select(childB);

        int roll;

        roll = rand()%100;
        if(roll <= CrossoverChance)
            Crossover(childA, childB);

        roll = rand()%100;
        if(roll <= MutationChance)
            Mutate(childA);

        roll = rand()%100;
        if(roll <= MutationChance)
            Mutate(childB);

        children.AddAsPerson(new Thinker(childA));
        children.AddAsPerson(new Thinker(childB));

    }

    Copy(children);
    children.Reset();
}

void Generation::Select(uint8_t params[PARAMETERS])
{
    int smallestScore = INF;
    for(int i = 0; i<people; i++)
        if(smallestScore > person[i]->score)
            smallestScore = person[i]->score;
    for(int i = 0; i<people; i++)
            person[i]->score -= smallestScore - 1;

    int totalScore = 0;
    for(int i=0; i<people; i++)
        totalScore += person[i]->score;

    int roll = 0;
    while(roll == 0) // we dont want roll to start with 0!
        roll = rand()%totalScore;

    //cout << "Total Score: "<< totalScore<< " - Roll: " << roll<< endl;

    int i;
    for(i = 0; roll > 0; i++) // i will never be out of bounds!
        roll -= person[i]->score;

    // Save the selected person's weights to the vector of parameters.
    person[i-1]->Encode(params);

}

void Generation::Crossover(uint8_t childA[PARAMETERS], uint8_t childB[PARAMETERS])
{
    //cout << "Crossing over: ";
    //for(int i=0; i<PARAMETERS; i++)
    //    cout << childA[i];
    //cout << endl;
    /// bits = 8 * PARAMETERS;

    int rollVect = rand()%PARAMETERS;
    int rollPos = rand()%5; // Symmetry. Max shift is 4 digits (0-4)

    // Swap all the data up to the vector position
    uint8_t temp;
    for(int i=0; i < rollVect; i++) {
        temp = childA[i];
        childA[i] = childB[i];
        childB[i] = temp;
    }

    // Now split the data in the position / bit, and exchange
    uint8_t maskAL = (childA[rollVect] << rollPos) >> rollPos;
    uint8_t maskAR = (childA[rollVect] >> (8 - rollPos)) >> (8 - rollPos);
    uint8_t maskBL = (childA[rollVect] << rollPos) >> rollPos;
    uint8_t maskBR = (childA[rollVect] << (8 - rollPos)) >> (8 - rollPos);

    childA[rollVect] = maskAL | maskBR;
    childB[rollVect] = maskBL | maskAR;

    //cout << endl << " As: ";
    //for(int i=0; i<PARAMETERS; i++)
    //    cout << childA[i];
    //cout << endl;
}

void Generation::Mutate(uint8_t params[PARAMETERS])
{
    //cout << "Mutating: ";
    //for(int i=0; i<PARAMETERS; i++)
    //    cout << params[i];
    //cout << endl;

/*
    int bits = 8 * PARAMETERS;
    int bitMutationChance = MutationChance / (bits)

    for(int j=0; j<PARAMETERS; j++) {
        int mask = 0;
        for(int i=0; i<8; i++) {
            float roll = (rand()%(100*1000))/1000.0f
            if(roll < bitMutationChance)
                mask += 1 << i;
        }
        // XOR with the mask of mutating bits...
        params[j] ^= mask;
    }*/


    int rollVect = rand()%PARAMETERS;
    int rollPos = rand()%8;

    // XOR with 00000001 shifted.
    params[rollVect] ^= 1 << rollPos;

    //cout << endl << " As: ";
    //for(int i=0; i<PARAMETERS; i++)
    //    cout << params[i];
    //cout << endl;

}
