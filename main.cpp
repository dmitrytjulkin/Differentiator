#include <stdio.h>
#include <assert.h>

#include "headers/tree.h"
#include "headers/differentiator.h"
#include "headers/dump.h"
#include "headers/nametable.h"
#include "headers/tokens.h"

#define MAIN_ARG "x"

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
// TODO make array of var that depend on x (dependencies)
// TODO think about d F/d x, d^2 F/d x^2
// TODO think about the way to finish reading formula

int main ()
{
    FILE* input_ptr = fopen (INPUT_FILENAME, "r");
    assert (input_ptr);

    nametable_t dependencies = {};
    InitNametable (&dependencies);

    token_array_t arg_queue = {};
    InitTokenArray (&arg_queue);

    tree_t* tree = InitTree ();
    tree = CreateTreeFromFile (input_ptr, &dependencies, &arg_queue);
    RunTexDump (TEXDUMP_FILENAME, tree);

    Optimize (tree);
    RunGraphDump (tree, GRAPH_DUMP_FILENAME, CMD_TO_RUN_TREE_DUMP);
    AddTexLine (TEXDUMP_FILENAME, tree->root, "Оптимизация формулы:");

    printf ("Size of tree: %zu\n", CountTreeSize (tree));

    for (size_t i = 0; i < arg_queue.size; ++i) {
        tree->root = DiffNode (tree->root, arg_queue.data[i].type.var.name,
                               &dependencies);
        AddTexLine (TEXDUMP_FILENAME, tree->root, "Дифференцирование формулы:");

        Optimize (tree);
        RunGraphDump (tree, DER_GRAPH_DUMP_FILENAME, CMD_TO_RUN_DER_TREE_DUMP);
        AddTexLine (TEXDUMP_FILENAME, tree->root, "И снова оптимизация формулы:");

        printf ("Size of der_tree: %zu\n", CountTreeSize (tree));
    }

    FinishTex (TEXDUMP_FILENAME);

    DestroyNametable (&dependencies);
    DestroyTokenArray (&arg_queue);
    fclose (input_ptr);
    FreeTree (tree);

    printf (GREEN "through the code and directories, "
            "i alone am the programmer one\n" COLOR_RESET);
}
