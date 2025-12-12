#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"


size_t CountNode (node_t* node);


tree_t* InitTree ()
{
    tree_t* tree = (tree_t*) calloc (1, sizeof (tree_t));
    assert (tree != NULL);

    tree->root = InitNode ();
    tree->size = 0;

    return tree;
}

node_t* InitNode ()
{
    node_t* node = (node_t*) calloc (1, sizeof (node_t));
    assert (node != NULL);

    node->parent = NULL;

    strcpy (node->data.var, "");

    node->expr = VAR;

    L = NULL;
    R = NULL;

    return node;
}

node_t* NewNode (type_of_expr expression, data_t value,
                 node_t* left_node, node_t* right_node)
{
    node_t* parent_node = InitNode ();

    parent_node->left = left_node;
    parent_node->right = right_node;

    parent_node->expr = expression;
    parent_node->data = value;

    return parent_node;
}

data_t DeleteNodeAndRetData (node_t* parent, child_node_t dir_of_child)
{
    assert (parent != NULL);

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

void GiveNodesTheirParents (node_t* node)
{
    assert (node != NULL);

    if (L != NULL) {
        L->parent = node;

        GiveNodesTheirParents (L);
    }

    if (R != NULL) {
        R->parent = node;

        GiveNodesTheirParents (R);
    }
}

node_t* FindRoot (node_t* node)
{
    if (node->parent != NULL)
        return FindRoot (node->parent);

    return node;
}

size_t CountTreeSize (tree_t* tree)
{
    assert (tree != NULL);

    tree->size = CountNode (tree->root);

    return tree->size;
}

size_t CountNode (node_t* node)
{
    assert (node != NULL);

    size_t current_size = 1;

    if (R != NULL)
        current_size += CountNode (R);

    if (L != NULL)
        current_size += CountNode (L);

    return current_size;
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
        FreeNode (L);
        FreeNode (R);

    } else
        return;
}
