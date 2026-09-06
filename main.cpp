#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "parse_tree/tree.h"
#include "differentiator/differentiator.h"
#include "build_dumps/dump.h"
#include "work_with_nametable/nametable.h"
#include "work_with_token_array/tokens.h"

const int INIT_SIZE = 100;

#define DEFAULT_INPUT_FILENAME         "input.txt"
#define DEFAULT_OUTPUT_FILENAME        "output.txt"

#define TEX_DUMP_FILENAME       "tex_dump/tree.tex"

#define GRAPH_DUMP_DIR         "graph_dump/"
#define TREE_GRAPH_DUMP        "tree_graph_dump"
#define TREE_GRAPH_DUMP_INPUT  GRAPH_DUMP_DIR TREE_GRAPH_DUMP ".dot"
#define TREE_GRAPH_DUMP_OUTPUT GRAPH_DUMP_DIR TREE_GRAPH_DUMP ".svg"

#define RUN_GRAPH_DUMP         "dot -Tsvg "
#define RUN_TREE_GRAPH_DUMP    RUN_GRAPH_DUMP TREE_GRAPH_DUMP_INPUT \
                               " -o " TREE_GRAPH_DUMP_OUTPUT

void AnalyzeTree (tree_t* tree);
void AnalyzeDerTree (tree_t* tree, nametable_t* dependencies,
                     token_array_t* arg_queue);

// TODO correct and improve optimisation
// TODO think about containing constants
// TODO make "input_to_tree" capable of parsing "(d^n y)"

int main (int argc, char* argv[])
{
    FILE* input_ptr = NULL;
    char output_filename[INIT_SIZE] = "";

    if (argc == 1)
        input_ptr = fopen (DEFAULT_INPUT_FILENAME, "r");

    else
        input_ptr = fopen (argv[1], "r");

    if (argc <= 2)
        strcpy (output_filename, DEFAULT_OUTPUT_FILENAME);

    else
        strcpy (output_filename, argv[2]);

    assert (input_ptr);

    nametable_t dependencies = {};
    InitNametable (&dependencies);

    token_array_t arg_queue = {};
    InitTokenArray (&arg_queue);

    tree_t* tree = InitTree ();
    tree = CreateTreeFromFile (input_ptr, &dependencies, &arg_queue);

    RunTexDump (TEX_DUMP_FILENAME, tree);
    AnalyzeTree (tree);
    AnalyzeDerTree (tree, &dependencies, &arg_queue);
    FinishTex (TEX_DUMP_FILENAME);

    FromTreeToFormula (output_filename, tree);

    DestroyNametable (&dependencies);
    DestroyTokenArray (&arg_queue);
    fclose (input_ptr);
    FreeTree (tree);

    printf (GREEN "through the code and directories, "
            "i alone am the programmer one\n" COLOR_RESET);
}

void AnalyzeTree (tree_t* tree)
{
    assert (tree);

    Optimize (tree);
    AddTexLine (TEX_DUMP_FILENAME, tree->root, "Оптимизация формулы:");

    RunGraphDump (tree, TREE_GRAPH_DUMP_INPUT, RUN_TREE_GRAPH_DUMP);
    printf ("Size of tree: %zu\n", CountTreeSize (tree));
}

void AnalyzeDerTree (tree_t* tree, nametable_t* dependencies,
                    token_array_t* arg_queue)
{
    assert (tree);
    assert (arg_queue);
    assert (dependencies);

    char input_file[INIT_SIZE] = "";
    char output_file[INIT_SIZE] = "";
    char run_graph_dump[INIT_SIZE] = "";

    for (size_t i = 0; i < arg_queue->size; ++i) {
        tree->root = DiffNode (tree->root, arg_queue->data[i].type.var.name, dependencies);
        AddTexLine (TEX_DUMP_FILENAME, tree->root, "Дифференцирование формулы:");

        Optimize (tree);
        AddTexLine (TEX_DUMP_FILENAME, tree->root, "И снова оптимизация формулы:");

        snprintf (input_file, INIT_SIZE, GRAPH_DUMP_DIR "%zuder_" TREE_GRAPH_DUMP ".dot", i + 1);
        snprintf (output_file, INIT_SIZE, GRAPH_DUMP_DIR "%zuder_" TREE_GRAPH_DUMP ".svg", i + 1);
        snprintf (run_graph_dump, INIT_SIZE, RUN_GRAPH_DUMP "%s -o %s",
                 input_file, output_file);

        RunGraphDump (tree, input_file, run_graph_dump);
        printf ("Size of der_tree: %zu\n", CountTreeSize (tree));
    }
}
