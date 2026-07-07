#pragma once

#include "tree.h"

node_t* Optimize (tree_t* tree);

node_t* DiffNode (node_t* node);
node_t* CopyNode (node_t* node);

tree_t* CreateTreeFromFile ();
char* ReadInput (FILE* input);
node_t* GetExpression (char* s);

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
