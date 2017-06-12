#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include <stdbool.h>
#include <limits.h>
#include "SPFIARGame.h"
#include "SPArrayList.h"
#include "unit_test_util.h"

#define SP_FIAR_GAME_N_ROWS 6
#define SP_FIAR_GAME_N_COLUMNS 7


typedef struct SPNode {
	struct SPNode* children[7]; // the word "struct" added before
	struct SPNode* parent; // the word "struct" added before
	int score;
	int score_vector[9];
	bool is_leaf;
	int level;
	int num_children;
	int col;
	SPFiarGame* board;
} SPNode;

typedef enum sp_node_message_t {
	SP_NODE_SUCCESS,
	SP_NODE_FAIL,
} SP_FIAR_NODE_MESSAGE;

SPNode* create_node(SPFiarGame* board, int level, SPNode* Parent);

SP_FIAR_NODE_MESSAGE delete_node(SPNode* node);

SP_FIAR_NODE_MESSAGE calculate_score(SPNode* node);

SP_FIAR_GAME_MESSAGE connect_child(SPNode* parent, SPNode* child);

SPNode* create_tree(SPFiarGame* board, int current_level, int max_depth, SPNode* Parent);

int update_score(SPNode* node);

#endif
