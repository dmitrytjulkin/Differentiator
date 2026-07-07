#include <stdio.h>
#include <assert.h>

#include "tree.h"

#define TEXDUMP_FILENAME         "tex_dump/tree.tex"
#define TREE_DUMP_FILENAME       "graph_dump/tree_graph_dump.dot"
#define DER_TREE_DUMP_FILENAME   "graph_dump/der_tree_graph_dump.dot"
#define CMD_TO_RUN_TREE_DUMP     "dot -Tsvg " TREE_DUMP_FILENAME \
                                 " -o graph_dump/tree_graph_dump.svg"
#define CMD_TO_RUN_DER_TREE_DUMP "dot -Tsvg " DER_TREE_DUMP_FILENAME \
                                 " -o graph_dump/der_tree_graph_dump.svg"

int main ()
{
    tree_t* tree = InitTree ();
    tree = CreateTreeFromFile ();

    RunTexDump (TEXDUMP_FILENAME, tree);

    Optimize (tree);

    AddTexLine (TEXDUMP_FILENAME, tree->root, "Оптимизация формулы:");

    tree_t* der_tree = InitTree ();
    der_tree->root = DiffNode (tree->root);

    AddTexLine (TEXDUMP_FILENAME, der_tree->root, "Дифференцирование формулы:");

    Optimize (der_tree);

    AddTexLine (TEXDUMP_FILENAME, der_tree->root, "И снова оптимизация формулы:");

    FinishTex (TEXDUMP_FILENAME);

    RunGraphDump (tree, TREE_DUMP_FILENAME, CMD_TO_RUN_TREE_DUMP);
    RunGraphDump (der_tree, DER_TREE_DUMP_FILENAME, CMD_TO_RUN_DER_TREE_DUMP);

    printf ("Size of tree: %zu\n", CountTreeSize (tree));
    printf ("Size of der_tree: %zu\n", CountTreeSize (der_tree));
    printf (GREEN "through the code and directories, "
            "i alone am the programmer one\n" COLOR_RESET);

    FreeTree (tree);
    FreeTree (der_tree);
}

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
