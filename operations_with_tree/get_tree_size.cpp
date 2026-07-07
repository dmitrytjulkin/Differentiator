#include <stdio.h>
#include <assert.h>

#include "../tree.h"

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
