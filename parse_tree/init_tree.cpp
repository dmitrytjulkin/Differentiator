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
    strcpy (node->data.var.name, "");
    node->expr = VAR;
    L = NULL;
    R = NULL;

    return node;
}
