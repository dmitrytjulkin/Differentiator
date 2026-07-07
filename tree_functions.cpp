#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

tree_t* InitTree ()
{
    tree_t* tree = (tree_t*) calloc (1, sizeof (tree_t));
    assert (tree);

    tree->root = InitNode ();
    tree->size = 0;

    return tree;
}

node_t* InitNode ()
{
    node_t* node = (node_t*) calloc (1, sizeof (node_t));
    assert (node);

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

// void GiveChildNodesParents (node_t* node)
// {
//     assert (node);
//
//     if (L != NULL) {
//         L->parent = node;
//
//         GiveChildNodesParents (L);
//     }
//
//     if (R != NULL) {
//         R->parent = node;
//
//         GiveChildNodesParents (R);
//     }
// }
//
// node_t* FindRoot (node_t* node)
// {
//     if (node->parent != NULL)
//         return FindRoot (node->parent);
//
//     return node;
// }

size_t CountTreeSize (tree_t* tree)
{
    assert (tree);

    tree->size = CountNodeSize (tree->root);

    return tree->size;
}

size_t CountNodeSize (node_t* node)
{
    assert (node);

    size_t current_size = 1;

    if (R != NULL)
        current_size += CountNodeSize (R);

    if (L != NULL)
        current_size += CountNodeSize (L);

    return current_size;
}

void FreeTree (tree_t* tree)
{
    assert (tree);

    FreeNode (tree->root);

    free (tree);
}

void FreeNode (node_t* node)
{
    if (node) {
        FreeNode (L);
        FreeNode (R);
        free (node);

    } else
        return;
}

void PrintNode (node_t* node)
{
    assert (node);

    printf ("\n");
    printf ("node: %p\n", node);
    printf ("his expression: %d\n", node->expr);

    if (node->expr == NUM)
        printf ("his data: %lg\n", node->data.num);

    else if (node->expr == VAR)
        printf ("his data: %s\n", node->data.var);

    else
        printf ("his data: %d\n", node->data.op);

    printf ("his left child: %p, right child %p\n", L, R);
    printf ("\n");
}
