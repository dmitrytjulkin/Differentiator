#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

#include "tree.h"

// TODO CreateFileFromTree ();

int main ()
{
    node_t* root = CreateTreeFromFile ();

    Optimize (root);  // doesn't optimize first operation

    node_t* derivative_root = d (root);

    Optimize (derivative_root);

    RunGraphDump (root, "tree_graph_dump.dot",
                  "dot -Tsvg tree_graph_dump.dot -o tree_graph_dump.svg");

    RunGraphDump (derivative_root, "der_tree_graph_dump.dot",
                  "dot -Tsvg der_tree_graph_dump.dot -o der_tree_graph_dump.svg");

    printf (GREEN "through the code and directories, "
            "i alone am the programmer one\n" COLOR_RESET);

//     printf (GREEN "PASSED in line %d\n" COLOR_RESET, __LINE__);
//     printf ("he:%p\n"
//         "his left child: %p, right child: %p\n"
//         "his type: %d, his data: %lg\n\n",
//         node, node->left, node->right, node->expr, node->data.num);

    FreeTree (root);
}

char* ReadInput (FILE* input)
{
    assert (input != NULL);

    struct stat input_data = {};

    fstat (fileno(input), &input_data);
    size_t size = (size_t) input_data.st_size;

    char* input_array = (char *) calloc (size + 10, sizeof(char));

    assert (input_array != NULL);

    fread (input_array, sizeof (char), size, input);

    return input_array;
}

bool IsZero (double a)
{
    if (fabs(a) < 1e-4)
        return 1;

    return 0;
}


