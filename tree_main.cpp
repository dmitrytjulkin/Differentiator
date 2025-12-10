#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "tree.h"

int main ()
{
    tree_t* tree = InitTree ();

    tree = CreateTreeFromFile ();

    Optimize (tree);  // FIXME doesn't optimize first operation

    tree_t* der_tree = InitTree ();

    der_tree->root = d (tree->root);

    Optimize (der_tree);

    RunGraphDump (tree, "tree_graph_dump.dot",
                  "dot -Tsvg tree_graph_dump.dot -o tree_graph_dump.svg");

    RunGraphDump (der_tree, "der_tree_graph_dump.dot",
                  "dot -Tsvg der_tree_graph_dump.dot -o der_tree_graph_dump.svg");

    RunTexDump (tree, der_tree);

    printf (GREEN "through the code and directories, "
            "i alone am the programmer one\n" COLOR_RESET);

//     printf (GREEN "PASSED in line %d\n" COLOR_RESET, __LINE__);
//     printf ("he:%p\n"
//         "his left child: %p, right child: %p\n"
//         "his type: %d, his data: %lg\n\n",
//         node, node->left, node->right, node->expr, node->data.num);

    FreeTree (tree);
    FreeTree (der_tree);
}

tree_t* CreateTreeFromFile ()
{
    FILE* input_ptr = fopen ("input.txt", "r");

    assert (input_ptr != NULL);

    char* input_array = ReadInput (input_ptr);

    assert (input_array != NULL);

    tree_t* tree = InitTree ();

    tree->root = GetG (input_array);

    fclose (input_ptr);
    free (input_array);

    return tree;
}

bool IsZero (double a)
{
    if (fabs(a) < 1e-4)
        return 1;

    return 0;
}
