#include <stdio.h>
#include <assert.h>

#include "../headers/tree.h"

void FindLeftmostLeaf (node_t* node)
{
    assert (node);

    if (node->left == NULL)
        return node;

    return FindLeftmostLeaf (node->left);
}
