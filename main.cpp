#include <stdio.h>
#include <assert.h>

#include "headers/tree.h"
#include "headers/differentiator.h"
#include "headers/dump.h"
#include "headers/nametable.h"
#include "headers/tokens.h"

const int INIT_SIZE = 100;

#define INPUT_FILENAME         "input.txt"

#define TEXDUMP_FILENAME       "tex_dump/tree.tex"

#define GRAPH_DUMP_DIR         "graph_dump/"
#define TREE_GRAPH_DUMP        "tree_graph_dump"
#define TREE_GRAPH_DUMP_INPUT  GRAPH_DUMP_DIR TREE_GRAPH_DUMP ".dot"
#define TREE_GRAPH_DUMP_OUTPUT GRAPH_DUMP_DIR TREE_GRAPH_DUMP ".svg"

#define RUN_GRAPH_DUMP         "dot -Tsvg "
#define RUN_TREE_GRAPH_DUMP    RUN_GRAPH_DUMP TREE_GRAPH_DUMP_INPUT \
                               " -o " TREE_GRAPH_DUMP_OUTPUT

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
    AddTexLine (TEXDUMP_FILENAME, tree->root, "Оптимизация формулы:");

    RunGraphDump (tree, TREE_GRAPH_DUMP_INPUT, RUN_TREE_GRAPH_DUMP);
    printf ("Size of tree: %zu\n", CountTreeSize (tree));

    char der_tree_graph_dump_input[INIT_SIZE] = "";
    char der_tree_graph_dump_output[INIT_SIZE] = "";
    char run_der_tree_graph_dump[INIT_SIZE] = "";

    for (size_t i = 0; i < arg_queue.size; ++i) {
        tree->root = DiffNode (tree->root, arg_queue.data[i].type.var.name,
                               &dependencies);
        AddTexLine (TEXDUMP_FILENAME, tree->root, "Дифференцирование формулы:");

        Optimize (tree);
        AddTexLine (TEXDUMP_FILENAME, tree->root, "И снова оптимизация формулы:");

        sprintf (der_tree_graph_dump_input, GRAPH_DUMP_DIR "%dder_" TREE_GRAPH_DUMP ".dot", i);
        sprintf (der_tree_graph_dump_output, GRAPH_DUMP_DIR "%dder_" TREE_GRAPH_DUMP ".svg", i);
        sprintf (run_der_tree_graph_dump, RUN_GRAPH_DUMP "%s -o " GRAPH_DUMP_DIR "%s",
                 der_tree_graph_dump_input, der_tree_graph_dump_output);
        RunGraphDump (tree, der_tree_graph_dump_input, run_der_tree_graph_dump);
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

