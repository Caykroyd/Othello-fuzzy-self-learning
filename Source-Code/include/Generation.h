#ifndef GENERATION_H
#define GENERATION_H

#include <iostream>
#include "Thinker.h"
#include "Board.h"

#define PEOPLE 100
#define TRAINERS 5

using namespace std;

class Generation
{
    public:
        Generation();
        ~Generation();

        void Reset();
        void Copy(const Generation &original);
        void Randomize(); // Initializes a population of players with random weights.

        void Tournament(); // Each person plays games against each other person.
        void Training(); // Each person plays games against all coaches.
        float ReferenceMatch(Thinker &t, int representatives, int &minScore, int &maxScore);

        void Evolve(); // Creates the next Generation from the current, based on the current fitness (score of the players).

        void AddAsPerson(Thinker *thinker); // Adds a Thinker to the population.
        void AddAsTrainer(Thinker *thinker); // Adds a Thinker to the population.
        Thinker* GetPlayer(int i);
        Thinker* GetTrainer(int i);
    protected:

        void Match(Thinker *black, Thinker *white); // Runs a match between two Thinkers.

        void Select(uint8_t params[PARAMETERS]); // Randomly selects a Thinker from the population based on their fitness (score).
        void Crossover(uint8_t childA[PARAMETERS], uint8_t childB[PARAMETERS]); // Yields 2 children from the crossover of a father and a mother, based on probability.
        void Mutate(uint8_t params[PARAMETERS]); // Mutates a Thinker's parameters.

        int people; // The number of people in the population (persons), with a max of PEOPLE.
        int trainers;
        Thinker *person[PEOPLE]; // The population of Thinkers.
        Thinker *trainer[TRAINERS]; // This generation's trainers.

        void GetBest(Thinker* best[], int amount); // Modifies best to include the n best players in the generation.

        static float MutationChance;
        static float CrossoverChance;
};

#endif // GENERATION_H
