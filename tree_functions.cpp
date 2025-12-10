#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

void ChooseNodeType (node_t* node, type_of_expr expression, data_t value);

tree_t* InitTree ()
{
    tree_t* tree = (tree_t*) calloc (1, sizeof (tree_t)); // FIXME

    tree->root = InitNode ();
    tree->size = 0;

    return tree;
}

node_t* InitNode ()
{
    node_t* node = (node_t*) calloc (1, sizeof (node_t));

    assert (node != NULL);

    node->parent = NULL;

    strcpy (node->data.func, "");

    node->expr = NO_EXPR;

    node->left = NULL;
    node->right = NULL;

    return node;
}

node_t* NewNode (type_of_expr expression, data_t value,
                 node_t* left_node, node_t* right_node)
{
    node_t* parent_node = InitNode ();

    parent_node->left = left_node;
    parent_node->right = right_node;

    ChooseNodeType (parent_node, expression, value);

    return parent_node;
}

void ChooseNodeType (node_t* node, type_of_expr expression, data_t value)
{
    assert (node != NULL);

    switch (expression) {
        case NUM:
            node->expr = NUM;
            node->data.num = value.num;

            break;

        case VAR:
            node->expr = VAR;
            strcpy (node->data.var, value.var);
            break;

        case OP:
            node->expr = OP;
            strcpy (node->data.op, value.op);

            break;

        case FUNC:
            node->expr = FUNC;
            strcpy (node->data.func, value.func);

            break;

        case NO_EXPR:
            break;

        default:
            break;
    }
}

data_t DeleteNodeAndRetData (node_t* parent, child_node_t dir_of_child)
{
    assert (parent != NULL);

    data_t ret_data = {.func = ""};

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

void FreeTree (tree_t* tree)
{
    assert (tree != NULL);

    FreeNode (tree->root);

    free (tree);
}

void FreeNode (node_t* node)
{
    if (node != NULL) {
        FreeNode (node->left);
        FreeNode (node->right);
    }
    else
        return;
}
