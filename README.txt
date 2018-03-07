Automatic Subtitles for Theatre Plays:

#SUMMARY#

This project was done for an elective "Machine Learning and Soft Computing" introductory course taken 
during my 3rd term (2016).
The project consisted on creating a system for playing the Othello board-game (a simplified version of
Go). In all previous studies analysed, the machine-player had been trained upon either large databases
of games or with the aid of already consolidated world-class game-playing machines.
The novelty of my approach, however, was in the use Genetic Algorithms to train a population of 
machine-players by playing against each other. This meant that there was NO external data, human or 
machine input. The training was totally unsupervised.

The chief concepts behind the algorithms were the Minimax Search algorithm, a Heuristic-Fuzzy
Evaluation function coupled with preprocessing of the othello board parameters, as well as the
a posteriori genetic evolution approach.

	- During each game: board configurations were preprocessed then evaluated with the fuzzy
function. This value was then back-fed with MiniMax to choose the best possible move.
	- At the end of many games: the performance of each machine-player was analysed based on the 
number of games won and the next generation was "genetically selected".


#Files#

1 - "Othello Fuzzy SelfLearning" [PORTUGUESE]: Contains the final report for the project.

2 - An "App" folder with the final executable & a save file (log.txt) for the machine-player state

3 - A "Source Code" folder with the source in C++.

