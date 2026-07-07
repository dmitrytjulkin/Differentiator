#include <stdio.h>
#include <assert.h>

#include "../headers/tree.h"

node_t* NewNode (type_of_expr expression, data_t value,
                 node_t* left_node, node_t* right_node)
{
    node_t* node = InitNode ();

    L          = left_node;
    R          = right_node;
    node->expr = expression;
    node->data = value;

    return node;
}

data_t DeleteNodeAndRetData (node_t* parent, child_node_t dir_of_child)
{
    assert (parent);

    data_t ret_data = {.var = ""};

    if (dir_of_child == LEFT) {
        ret_data = parent->left->data;
        parent->left = NULL;

        free (parent->left);

    } else {
        ret_data = parent->right->data;
        parent->right = NULL;

        free (parent->right);
    }

    return ret_data;
}
