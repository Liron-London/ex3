/*
 * SPFIARGame.c
 *
 *  Created on: Jun 1, 2017
 *      Author: galsadeh2
 */
#include "SPFIARGame.h"
#include <stdio.h>
#include <stdlib.h>

SPFiarGame* spFiarGameCreate(int historySize){
	if (historySize <= 0){
		return NULL;
	}
	SPFiarGame *board = malloc(sizeof(SPFiarGame));
	board->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	// allocation is not needed because arrays are from fixed size
	// board->tops = malloc(SP_FIAR_GAME_N_COLUMNS, sizeof(char));
	// board->gameBoard = calloc(SP_FIAR_GAME_N_COLUMNS * SP_FIAR_GAME_N_ROWS, sizeof(char));
	board->historySize = historySize;
	for (int i=0; i < SP_FIAR_GAME_N_COLUMNS; i++ ){
		for (int j=0; j < SP_FIAR_GAME_N_ROWS; j++){
			board->gameBoard[j][i] = SP_FIAR_GAME_EMPTY_ENTRY;
		}
		board->tops[i] = 0; // initialize tops
	}
	board->turn_array = spArrayListCreate(historySize);
	if (board->turn_array == NULL){
		spArrayListDestroy(board->turn_array);
		spFiarGameDestroy(board);
		return NULL;
	}
	return board;
}


SPFiarGame* spFiarGameCopy(SPFiarGame* src){
	SPFiarGame *board_copy = malloc(sizeof(SPFiarGame));
	board_copy->currentPlayer = src->currentPlayer;
	board_copy->historySize = src->historySize;
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++){
		board_copy->tops[i] = src->tops[i];
	}
	for (int i=0; i < SP_FIAR_GAME_N_COLUMNS; i++){
		for (int j=0; j < SP_FIAR_GAME_N_ROWS; j++){
			board_copy->gameBoard[j][i] = src->gameBoard[j][i];
		}
	}

	board_copy->turn_array = spArrayListCopy(src->turn_array);
	if (board_copy->turn_array == NULL){
		free(board_copy->turn_array);
		return NULL;
	}

	return board_copy;
}

/* Frees all memory allocation associated with a given game. If src==NULL
* the function does nothing.
*
* @param src - the source game
*/
void spFiarGameDestroy(SPFiarGame* src){
	if (src != NULL){
		spArrayListDestroy(src->turn_array);
		free(src);
	}
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
	col--;
	if (src == NULL || col < 0  || col > SP_FIAR_GAME_N_COLUMNS){
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	if (src->tops[col] == SP_FIAR_GAME_N_ROWS){
		return SP_FIAR_GAME_INVALID_MOVE;
	}
	char sign = '?';
	// change player's turn
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL){
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
		sign = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	else{
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
		sign = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}
	src->gameBoard[src->tops[col]][col] = sign;
	src->tops[col] += 1;
	if (spArrayListIsFull(src->turn_array) == true){
		spArrayListRemoveFirst(src->turn_array);
	}
	spArrayListAddLast(src->turn_array, col);

	return SP_FIAR_GAME_SUCCESS; // NOT IN THE INSTRUCTIONS -- need to check this
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col){
	if (src->tops[col] == SP_FIAR_GAME_N_ROWS){
		return false;
	}
	return true;
}

/**
 * Removes a disc that was put in the previous move and changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param src - The source game
 * @return
 * SP_FIAR_GAME_INVALID_ARGUMENT - if src == NULL
 * SP_FIAR_GAME_NO_HISTORY       - if the user invoked this function more then
 *                                 historySize in a row.
 * SP_FIAR_GAME_SUCCESS          - on success. The last disc that was put on the
 *                                 board is removed and the current player is changed
 */
SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src){
	if (src == NULL){
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	if (spArrayListIsEmpty(src->turn_array)){
		return SP_FIAR_GAME_NO_HISTORY;
	}
	int last_move = spArrayListGetLast(src->turn_array); // get the last column player changed

	spArrayListRemoveLast(src->turn_array);
	src->gameBoard[src->tops[last_move]][last_move] = SP_FIAR_GAME_EMPTY_ENTRY;
	src->tops[last_move] = src->tops[last_move] - 1;

	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
	if (src == NULL){
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
		printf("|");
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
			printf("%c", src->gameBoard[SP_FIAR_GAME_N_ROWS - i - 1][j]);
			printf(" ");
		}
		printf("|\n");
	}
	printf("-----------------\n");
	printf(" 1 2 3 4 5 6 7 \n");
	return SP_FIAR_GAME_SUCCESS;
}


char spFiarGameGetCurrentPlayer(SPFiarGame* src){
	if (src == NULL){
		return SP_FIAR_GAME_EMPTY_ENTRY;
	}
	return src->currentPlayer;
}


char spFiarCheckWinner(SPFiarGame* src) {
	int tmp_score = 0; //calculates difference between #O-#X
	// iterate over all columns
	for (int i=0; i< SP_FIAR_GAME_N_COLUMNS; i++) {
		for (int j=0; j < SP_FIAR_GAME_N_ROWS - 3; j++) {
			tmp_score = 0;
			for (int k=0; k<4; k++) {
				if (src->gameBoard[j+k][i] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
					tmp_score += 1;
				}
				if (src->gameBoard[j+k][i] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
					tmp_score -= 1;
				}
			}
			if (tmp_score==4){
				return SP_FIAR_GAME_PLAYER_1_SYMBOL;
			}
			if (tmp_score==-4){
				return SP_FIAR_GAME_PLAYER_2_SYMBOL;
			}

		}
	}

	// iterate over all rows
	for (int i=0; i< SP_FIAR_GAME_N_ROWS; i++) {
		for (int j=0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			tmp_score = 0;
			for (int k=0; k<4; k++) {
				if (src->gameBoard[i][j+k] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
					tmp_score += 1;
				}
				if (src->gameBoard[i][j+k] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
					tmp_score -= 1;
				}
			}
			if (tmp_score==4){
				return SP_FIAR_GAME_PLAYER_1_SYMBOL;
			}
			if (tmp_score==-4){
				return SP_FIAR_GAME_PLAYER_2_SYMBOL;
			}
		}
	}

	// check 4 in up-diagonal
	for (int i=0; i < SP_FIAR_GAME_N_ROWS - 3; i++){
		for (int j=0; j <  SP_FIAR_GAME_N_COLUMNS - 3; j++){
			tmp_score = 0;
			for (int k=0; k<4; k++){
				if (src->gameBoard[i+k][j+k] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
					tmp_score += 1;
				}
				if (src->gameBoard[i+k][j+k] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
					tmp_score -= 1;
				}
			}
			if (tmp_score==4){
				return SP_FIAR_GAME_PLAYER_1_SYMBOL;
			}
			if (tmp_score==-4){
				return SP_FIAR_GAME_PLAYER_2_SYMBOL;
			}
		}
	}

	for (int i=3; i < SP_FIAR_GAME_N_ROWS; i++){
		for (int j=0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++){
			tmp_score = 0;
			for (int k=0; k<4; k++){
				if (src->gameBoard[i-k][j+k] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
					tmp_score += 1;
				}
				if (src->gameBoard[i-k][j+k] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
					tmp_score -= 1;
				}
			}
			if (tmp_score==4){
				return SP_FIAR_GAME_PLAYER_1_SYMBOL;
			}
			if (tmp_score==-4){
				return SP_FIAR_GAME_PLAYER_2_SYMBOL;
			}
		}
	}

	for (int i=0; i<SP_FIAR_GAME_N_ROWS; i++) {
		for (int j=0; j<SP_FIAR_GAME_N_COLUMNS;j++){
			if (src->gameBoard[i][j] == SP_FIAR_GAME_EMPTY_ENTRY){
				return '\0';
			}
		}
	}

	return SP_FIAR_GAME_TIE_SYMBOL;
}


/**
char spFiarCheckWinner(SPFiarGame* src){
	// check 4 in a column
	for (int i=0; i < SP_FIAR_GAME_N_COLUMNS; i++){
		if (src->tops[i] > 4){
			if (src->gameBoard[i][src->tops[i]] == src->gameBoard[i][src->tops[i-1]] &&
					src->gameBoard[i][src->tops[i]] == src->gameBoard[i][src->tops[i-2]] &&
					src->gameBoard[i][src->tops[i]] == src->gameBoard[i][src->tops[i-3]]){
				return src->gameBoard[i][src->tops[i]];
			}
		}
	}

	// check 4 in a row
	for (int i=0; i < SP_FIAR_GAME_N_ROWS; i++){
		for (int j=0; j < SP_FIAR_GAME_N_COLUMNS - 4; j++){
			if (src->gameBoard[i][src->tops[i]] == src->gameBoard[i+1][src->tops[i]] &&
					src->gameBoard[i][src->tops[i]] == src->gameBoard[i+2][src->tops[i]] &&
					src->gameBoard[i][src->tops[i]] == src->gameBoard[i+3][src->tops[i]]&&
					src->gameBoard[i][src->tops[i]] != SP_FIAR_GAME_EMPTY_ENTRY){
				return src->gameBoard[i][src->tops[i]];
			}
		}
	}

	// check 4 in up-diagonal
	for (int i=0; i < SP_FIAR_GAME_N_COLUMNS - 4; i++){
		for (int j=0; j < SP_FIAR_GAME_N_ROWS - 4; j++){
			if (src->gameBoard[i][j] == src->gameBoard[i+1][j+1] &&
					src->gameBoard[i][j] == src->gameBoard[i+2][j+2] &&
					src->gameBoard[i][j] == src->gameBoard[i+3][j+3]&&
					src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY){
				return src->gameBoard[i][j];
			}
		}
	}

	// check 4 in down-diagonal
	for (int i=4; i < SP_FIAR_GAME_N_COLUMNS; i++){ // maybe should be change to 3
		for (int j=4; j < SP_FIAR_GAME_N_ROWS; j++){
			if (src->gameBoard[i][j] == src->gameBoard[i-1][j-1] &&
					src->gameBoard[i][j] == src->gameBoard[i-2][j-2] &&
					src->gameBoard[i][j] == src->gameBoard[i-3][j-3]&&
					src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY){
				return src->gameBoard[i][j];
			}
		}
	}

	//check tie
	for (int i=0; i < SP_FIAR_GAME_N_COLUMNS; i++){
		if (src->tops[i] != SP_FIAR_GAME_N_COLUMNS){
			return '\0';
		}
	}

	return SP_FIAR_GAME_TIE_SYMBOL;
}
*/




