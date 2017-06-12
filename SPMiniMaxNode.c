/*
 * SPMiniMaxNode.c
 *
 *  Created on: Jun 5, 2017
 *      Author: galsadeh2
 */
#include "SPMiniMaxNode.h"
#include <stdlib.h>

SPNode* create_tree(SPFiarGame* board, int current_level, int max_depth, SPNode* Parent) {
	//recursion halting condition
	if (current_level == max_depth){
		return NULL; // leaves children are NULLs
	}
	SPNode* root = create_node(board, current_level, Parent);

	// iterate over columns
	for(int i=0; i<7; i++) {
		SPFiarGame* board_copy = spFiarGameCopy(board);
		if (spFiarGameIsValidMove(board_copy, i) == true) {
			spFiarGameSetMove(board_copy, i);
			SPNode* child = create_tree(board_copy, root->level + 1, max_depth, root);
			//printf("a child is born!\n");
			if (child!= NULL){
				child->col = i;
				child->board = board_copy;
				connect_child(root, child);
				//spFiarGamePrintBoard(child->board);
				//printf("child score: %i%s", child->score, "\n");
			}
			//spFiarGameDestroy(board_copy);
		}
	}
	return root;
}

SP_FIAR_GAME_MESSAGE connect_child(SPNode* parent, SPNode* child){
	parent->children[parent->num_children] = child;
	parent->num_children ++;
	parent->is_leaf = false;
	return SP_NODE_SUCCESS;
}

// node is the root of the tree
int update_score(SPNode* node) {
	int max_score = -500;
	int min_score = 500;

	if (node->is_leaf) {
		calculate_score(node);
	//	printf("node score is: %i \n", node->score);
		return node->score;
	}

	else {
		if (node->level %2 == 1){
			for (int i=0; i<7; i++){
				if (node->children[i] != NULL && update_score(node->children[i]) >= max_score){
					max_score = node->children[i]->score;
					node->score = max_score;
				}
			}
			//printf("Current level is %i \n", node->level);
			//printf("max score is: %i \n", max_score);
			return max_score;
		}

		else{
			for (int i=0; i<7; i++){
				if (node->children[i] != NULL && update_score(node->children[i]) <= min_score){
					min_score = node->children[i]->score;
					node->score = min_score;
				}
			}
			//printf("Current level is %i \n", node->level);
			//printf("min score is: %i \n", min_score);
			return min_score;
		}
	}
}


SPNode* create_node(SPFiarGame* board, int level, SPNode* Parent){
	SPNode* node = malloc(sizeof(SPNode));
	//node->children = (SPNode*) calloc(7, sizeof(SPNode));
	node->parent = Parent;
	node->num_children = 0;
	node->score = 0;
	node->is_leaf = true;
	node->level = level;
	node->board = board;
	node->col = -1; //NULL doesn't work
	return node;
}

SP_FIAR_NODE_MESSAGE delete_node(SPNode* node){
	if (node == NULL){
		return SP_NODE_FAIL;
	}
	free(node->children);
	free(node);
	return SP_NODE_SUCCESS;
}

//updates the score vector (that counts # of sequences of size 0/1/2/3 and updates the score accordingly)
SP_FIAR_NODE_MESSAGE calculate_score(SPNode* node) {

	int tmp_score = 0; //calculates difference between #O-#X
	// iterate over all columns
	for (int i=0; i< SP_FIAR_GAME_N_COLUMNS; i++) {
		for (int j=0; j < SP_FIAR_GAME_N_ROWS - 3; j++) {
			tmp_score = 0;
			for (int k=0; k<4; k++) {
				if (node->board->gameBoard[j+k][i] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
					tmp_score += 1;
				}
				if (node->board->gameBoard[j+k][i] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
					tmp_score -= 1;
				}
			}
			if (tmp_score!=0){
				//printf("col added\n");
			}
			node->score_vector[tmp_score+4] ++;
		}
	}

	// iterate over all rows
	for (int i=0; i< SP_FIAR_GAME_N_ROWS; i++) {
		for (int j=0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			tmp_score = 0;
			for (int k=0; k<4; k++) {
				if (node->board->gameBoard[i][j+k] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
					tmp_score += 1;
				}
				if (node->board->gameBoard[i][j+k] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
					tmp_score -= 1;
				}
			}
			if (tmp_score!=0){
				//printf("row added\n");
			}
			node->score_vector[tmp_score+4] ++;
		}
	}


	// check 4 in up-diagonal
	for (int i=0; i < SP_FIAR_GAME_N_ROWS - 3; i++){
		for (int j=0; j <  SP_FIAR_GAME_N_COLUMNS - 3; j++){
			tmp_score = 0;
			for (int k=0; k<4; k++){
				if (node->board->gameBoard[i+k][j+k] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
					tmp_score += 1;
				}
				if (node->board->gameBoard[i+k][j+k] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
					tmp_score -= 1;
				}
			}
			if (tmp_score!=0){
				//printf("up-diagonal added\n");
			}
			node->score_vector[tmp_score+4] ++;
		}
	}

	for (int i=3; i < SP_FIAR_GAME_N_ROWS; i++){
		for (int j=0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++){
			tmp_score = 0;
			for (int k=0; k<4; k++){
				if (node->board->gameBoard[i-k][j+k] == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
					tmp_score += 1;
				}
				if (node->board->gameBoard[i-k][j+k] == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
					tmp_score -= 1;
				}
			}
			if (tmp_score!=0){
			//	printf("down-diagonal added\n");
			}
			node->score_vector[tmp_score+4] ++;
		}
	}
	spFiarGamePrintBoard(node->board);
	printf("score vector is: ");
	for (int i = 0; i < 7; i++) {
		printf("%i ", node->score_vector[i]);
	}
	printf("\n");

	int final_score=0;
	final_score = node->score_vector[0]*INT_MIN + node->score_vector[1]*(-5) + node->score_vector[2]*(-2) + node->score_vector[3]*(-1) +\
			node->score_vector[5]*(1) + node->score_vector[6]*(2) + node->score_vector[7]*(5) + node->score_vector[8]*(INT_MAX);
	printf("final score is: %i \n", final_score);
	node->score = final_score;

	return SP_NODE_SUCCESS;
}
