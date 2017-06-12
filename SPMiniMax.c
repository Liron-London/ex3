/*
 * SPMiniMax.c
 *
 *  Created on: Jun 5, 2017
 *      Author: galsadeh2
 */

#include "SPMiniMax.h"

int spMinimaxSuggestMove(SPFiarGame* currentGame,unsigned int maxDepth){
	SPNode* root = create_tree(currentGame, 0, maxDepth, NULL);
	int tmp_score = -200;
	int root_score = 0;// score of the root
	int move_recommendation = NULL;
	root_score = update_score(root);
	printf("%i", root_score); // for debug

	for (int i = 0; i < 7; i++) {
		if (root->children[i] == NULL) {
			return move_recommendation;
		}
		else {
			if (root->children[i]->score > tmp_score && root->board->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL){
				tmp_score = root->children[i]->score;
				move_recommendation = root->children[i]->col;
			}

			if (root->children[i]->score < tmp_score && root->board->currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL){
				tmp_score = root->children[i]->score;
				move_recommendation = root->children[i]->col;
			}
		}
	}

	return move_recommendation;
}
