#include <stdio.h>
#include <assert.h>

#include "tree.h"

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
