#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "tree.h"

// TODO CreateFileFromTree ();

int main ()
{
    node_t* root = CreateTreeFromFile ();

    Optimize (root);  // FIXME doesn't optimize first operation

    node_t* derivative_root = d (root);

    // Optimize (derivative_root);

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

node_t* CreateTreeFromFile ()
{
    FILE* input_ptr = fopen ("input.txt", "r");

    assert (input_ptr != NULL);

    char* input_array = ReadInput (input_ptr);

    assert (input_array != NULL);

    node_t* root = GetG (&input_array);

    fclose (input_ptr);
    // free (input_array);

    return root;
}

char* ReadInput (FILE* input)
{
    assert (input != NULL);

    struct stat input_data = {};

    fstat (fileno(input), &input_data);
    size_t size = (size_t) input_data.st_size;

    char* input_array = (char *) calloc (size + EXTRA_SIZE, sizeof(char));

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
