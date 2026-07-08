#pragma once

#define GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

#include "func_and_op.h"

#define R node->right
#define L node->left

enum child_node_t {
    LEFT =  -1,
    RIGHT = 1
};

struct node_t {
    node_t*      parent;
    type_of_expr expr;
    data_t       data;
    node_t* left;
    node_t* right;
};

struct tree_t {
    node_t* root;
    size_t size;
};

tree_t* InitTree ();
node_t* InitNode ();

node_t* NewNode (type_of_expr expression, data_t value,
                node_t* left_node, node_t* right_node);
data_t  DeleteNodeAndRetData (node_t* node, child_node_t dir_of_child);

size_t  CountTreeSize (tree_t* tree);
size_t  CountNodeSize (node_t* node);

void    FreeTree (tree_t* tree);
void    FreeNode (node_t* node);

void    PrintNode (node_t* node);

