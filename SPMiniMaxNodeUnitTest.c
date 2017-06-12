/*
 * SPFIARMiniMaxNodeUnitTest.c
   SPFIARMiniMaxNodeUnitTest.c
 *
 *  Created on: Jun 6, 2017
 *      Author: galsadeh2
 */
#include "unit_test_util.h"
#include "SPMiniMax.h"
#include <stdlib.h>
#include "SPMiniMaxNode.h"
#include "SPFIARGame.h"

static bool create_node_test() {
	SPFiarGame* board;
	board = spFiarGameCreate(20);
	SPNode* node = create_node(board, 0, NULL);
	ASSERT_TRUE(node != NULL);
	//printf("%s %i level: %i \n", " is leaf: ", node->is_leaf, node->level);
	return true;
}

/**
 * 	struct SPNode* children[7]; // the word "struct" added before
	struct SPNode* parent; // the word "struct" added before
	int score;
	int score_vector[7];
	bool is_leaf;
	int level;
	int num_children;
	int col;
	SPFiarGame* board;
} SPNode;
 */

static bool create_tree_test() {
	SPFiarGame* board;
	board = spFiarGameCreate(20);
	SPNode* root = create_tree(board, 0, 2, NULL);
	ASSERT_TRUE(root != NULL);
	return true;
}

static bool score_test() {
	printf("In the score_test function\n");
	SPFiarGame* board = spFiarGameCreate(20);
	spFiarGameSetMove(board, 1);
	spFiarGameSetMove(board, 6);
	spFiarGameSetMove(board, 1);
	spFiarGameSetMove(board, 3);
	spFiarGameSetMove(board, 1);
	spFiarGameSetMove(board, 6);

	SPNode* root = create_tree(board, 0, 3, NULL);
	update_score(root);
	printf("The root score is %i \n", root->score);
	return true;
}



int main() {
	RUN_TEST(create_node_test);
	RUN_TEST(create_tree_test);
	RUN_TEST(score_test);
	return 0;
}

