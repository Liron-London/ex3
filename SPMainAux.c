/*
 * SPMainAux.c
 *
 *  Created on: 10 June 2017
 *      Author: lironlondon
 */
#include "SPMainAux.h"


int initialize() {
	char* difficulty = " ";
	printf("Please enter the difficulty level between [1-7]:\n");
	scanf("%s", difficulty);

	while(!spParserIsInt(difficulty) || atoi(difficulty) > 7 || atoi(difficulty) < 1) {
		printf("Error: invalid level (should be between 1 to 7\n");
		printf("Please enter the difficulty level between [1-7]:\n");
		scanf("%s", difficulty);
	}
	return atoi(difficulty);
}

bool is_between_1_to_7(int val) {
	if (val >= 1 && val <= 7) {
		return true;
	}
	return false;
}

// function initializes board and runs a while loop of commands until the game is finished or the
// user enters the command "quit"

int gameplay(int difficulty) {
	SPFiarGame* board = spFiarGameCreate(20);
	char* command_str = " ";
	SPCommand command;

	bool all_commnads = true; //determines whether all the commands can be chosen or just undo/quit/restart

	//prompt user to enter command
	while (true) {
		printf("Please make the next move:\n");
		scanf("%s[^\n]", command_str);
		command = spParserPraseLine(command_str);

		if (command.cmd == SP_QUIT) {
			quit(board);
			all_commnads = true;
			return 0;
		}


		//add disc: cannot be done if game is over
		if (command.cmd == SP_ADD_DISC) {
			if (!all_commnads) {
				printf("Error: the game is over\n");
			}
			else {
				add_disc(board, command.arg, all_commnads);
			}
		}
		//restart
		if (command.cmd == SP_RESTART) {
			restart(board);
			all_commnads = true;
			return 0;
		}

		//if the command is invalid
		if (command.cmd == SP_INVALID_LINE){
			printf("Error: invalid command\n");
		}

		//undo move
		if (command.cmd == SP_UNDO_MOVE && all_commnads) {
			undo_move(board);
		}

		//suggest move
		if (command.cmd == SP_SUGGEST_MOVE && all_commnads){
			int suggest = spMinimaxSuggestMove(board, difficulty);
			printf("Suggested move: drop a disc to column %i\n", suggest);
		}

	//after the while - computer turn
		if (all_commnads) {
			computer_turn(difficulty, board);
		}
	}
}

void computer_turn(int difficulty, SPFiarGame* board) {
	int col_to_add = spMinimaxSuggestMove(board, difficulty);
	spFiarGameSetMove(board, col_to_add);
	printf("Computer move: add disc to column %i\n", col_to_add);
}

void quit(SPFiarGame* board) {
	printf("Exiting…\n");
	spFiarGameDestroy(board);
}

//returns the updated value of all_commands
bool add_disc(SPFiarGame* board, int col, bool all_commands) {
	//check if the col # is between 1-7 AND col is not full
	if (is_between_1_to_7(col)) {
		if(spFiarGameSetMove(board, col) == SP_FIAR_GAME_INVALID_MOVE) {
			printf("Error: column %i is full\n", col);
			return all_commands;
		}
		//check if player 1 won
		if (spFiarCheckWinner(board) == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
			printf ("Game over: you win\nPlease enter ‘quit’ to exit or"
					" ‘restart’ to start a new game!\n");
			return false;
		}
		//check if player 2 (computer) won
		if (spFiarCheckWinner(board) == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
			printf ("Game over: computer wins\nPlease enter ‘quit’ to exit or"
					" ‘restart’ to start a new game!\n");
			return false;
		}
		//check if tie
		if (spFiarCheckWinner(board) == SP_FIAR_GAME_TIE_SYMBOL) {
			printf ("Game over: it’s a tie\nPlease enter ‘quit’ to exit or "
					"‘restart’ to start a new game!\n");;
			return false;
		}
		return all_commands;
	}
	else {
		printf("Error: column number must be in range 1-7\n");
		return all_commands;
	}
}

void restart(SPFiarGame* board) {
	printf("Game restarted!\n");
	spFiarGameDestroy(board);
	int difficulty = initialize();
	gameplay(difficulty);
}

void undo_move(SPFiarGame* board) {
	int last_move = spArrayListGetLast((board->turn_array));
	if(spFiarGameUndoPrevMove(board)!=SP_FIAR_GAME_SUCCESS) {
		printf("Error: cannot undo previous move!\n");
	}
	else {
		printf("Remove disc: remove computer’s disc at column %i\n", last_move);
		last_move = spArrayListGetLast((board->turn_array));
		spFiarGameUndoPrevMove(board);
		printf("Remove disc: remove user’s disc at column %i\n", last_move);
	}
}
