#pragma once

#include "../parse_tree/tree.h"
#include "../work_with_nametable/nametable.h"
#include "../work_with_token_array/tokens.h"

node_t* Optimize (tree_t* tree);

node_t* DiffNode (node_t* node, const char* arg, nametable_t* dependencies);
node_t* CopyNode (node_t* node);

tree_t* CreateTreeFromFile (FILE* input_ptr, nametable_t* dependencies,
                            token_array_t* arg_queue);

#define OPTIMIZE_IF_NUM_OP_NUM(operation)                                \
    if (node->left->expr == NUM && node->right->expr == NUM) {           \
        node->expr = NUM;                                                \
                                                                         \
        node->data.num = DeleteNodeAndRetData (node, LEFT).num operation \
                         DeleteNodeAndRetData (node, RIGHT).num;         \
                                                                         \
        *is_tree_changed = true;                                         \
                                                                         \
        return node;                                                     \
    }

