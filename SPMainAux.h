#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#include "SPMiniMaxNode.h"
#include "SPMiniMax.h"
#include <stdio.h>
#define historySize 20;
#include <stdbool.h>

//put auxiliary functions and constants used by the main function here.

// getting difficulty level from user and returns it
int initialize();

// function initializes board and runs a while loop of commands until the game is finished or the user enters the command "quit"
int gameplay(int difficulty);


//computer turn
void computer_turn(int difficulty, SPFiarGame* board);

//quit
void quit(SPFiarGame* board);

//add disc: returns the updated value of all_commands
bool add_disc(SPFiarGame* board, int col, bool all_commands);

//restart
void restart(SPFiarGame* board);

//undo move - undoes both moves!
void undo_move(SPFiarGame* board);

#endif
