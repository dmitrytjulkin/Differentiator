#include <stdio.h>
#include <assert.h>

#include "../tree.h"

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
