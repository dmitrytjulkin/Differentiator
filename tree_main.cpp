#include <stdio.h>
#include <assert.h>

#include "tree.h"

int main ()
{
    tree_t* tree = InitTree ();

    tree = CreateTreeFromFile ();

    Optimize (tree);

    tree_t* der_tree = InitTree ();

    der_tree->root = DiffNode (tree->root);

    Optimize (der_tree);

    RunGraphDump (tree, "tree_graph_dump.dot",
                  "dot -Tsvg tree_graph_dump.dot -o tree_graph_dump.svg");

    RunGraphDump (der_tree, "der_tree_graph_dump.dot",
                  "dot -Tsvg der_tree_graph_dump.dot -o der_tree_graph_dump.svg");

    RunTexDump (tree, der_tree);

    printf (GREEN "through the code and directories, "
            "i alone am the programmer one\n" COLOR_RESET);

    FreeTree (tree);
    FreeTree (der_tree);
}

void PrintNode (node_t* node)
{
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
