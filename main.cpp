#include <stdio.h>
#include <assert.h>

#include "headers/tree.h"
#include "headers/differentiator.h"
#include "headers/dump.h"

#define INPUT_FILENAME           "input.txt"
#define TEXDUMP_FILENAME         "tex_dump/tree.tex"
#define GRAPH_DUMP_FILENAME       "graph_dump/tree_graph_dump.dot"
#define DER_GRAPH_DUMP_FILENAME   "graph_dump/der_tree_graph_dump.dot"
#define CMD_TO_RUN_TREE_DUMP     "dot -Tsvg " GRAPH_DUMP_FILENAME \
                                 " -o graph_dump/tree_graph_dump.svg"
#define CMD_TO_RUN_DER_TREE_DUMP "dot -Tsvg " DER_GRAPH_DUMP_FILENAME \
                                 " -o graph_dump/der_tree_graph_dump.svg"

// TODO correct and improve optimisation
// TODO think about containing constants

int main ()
{
    FILE* input_ptr = fopen (INPUT_FILENAME, "r");
    assert (input_ptr);

    tree_t* tree = InitTree ();
    tree = CreateTreeFromFile (input_ptr);

    RunTexDump (TEXDUMP_FILENAME, tree);

    // Optimize (tree);

    AddTexLine (TEXDUMP_FILENAME, tree->root, "Оптимизация формулы:");

//     tree_t* der_tree = InitTree ();
//     der_tree->root = DiffNode (tree->root);
//
//     AddTexLine (TEXDUMP_FILENAME, der_tree->root, "Дифференцирование формулы:");
//
//     Optimize (der_tree);
//
//     AddTexLine (TEXDUMP_FILENAME, der_tree->root, "И снова оптимизация формулы:");

    FinishTex (TEXDUMP_FILENAME);

    RunGraphDump (tree, GRAPH_DUMP_FILENAME, CMD_TO_RUN_TREE_DUMP);
    // RunGraphDump (der_tree, DER_GRAPH_DUMP_FILENAME, CMD_TO_RUN_DER_TREE_DUMP);

    printf ("Size of tree: %zu\n", CountTreeSize (tree));
    // printf ("Size of der_tree: %zu\n", CountTreeSize (der_tree));
    printf (GREEN "through the code and directories, "
            "i alone am the programmer one\n" COLOR_RESET);

    fclose (input_ptr);
    FreeTree (tree);
    // FreeTree (der_tree);
}
