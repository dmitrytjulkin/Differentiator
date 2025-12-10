#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "tree.h"

void PrintNum (node_t* node, FILE* output_ptr);
void PrintFunc (node_t* node, FILE* output_ptr);
void PrintOp (node_t* node, FILE* output_ptr);
void PrintVar (node_t* node, FILE* output_ptr);

void RunGraphDump (tree_t* tree, const char* name_of_file,
                   const char* cmd_to_launch_graph_dump)
{
    assert (tree != NULL);
    assert (name_of_file != NULL);
    assert (cmd_to_launch_graph_dump != NULL);

    ClearDump (name_of_file);

    FILE* output_ptr = fopen (name_of_file, "a");

    assert (output_ptr != NULL);

    PrintTreeInGraphDump (tree->root, output_ptr);

    fclose (output_ptr);

    system (cmd_to_launch_graph_dump);
}

void PrintTreeInGraphDump (node_t* root, FILE* output_ptr)
{
    assert (root != NULL);
    assert (output_ptr != NULL);

    fprintf (output_ptr, "digraph demo { \n"
            "\t rankdir = TB; \n"
            "\t splines = ortho;\n"
            "\t node [shape = plaintext];\n"
            "\n");

    if (root->expr == NUM)
        PrintNum (root, output_ptr);

    if (root->expr == FUNC)
        PrintFunc (root, output_ptr);

    if (root->expr == OP)
        PrintOp (root, output_ptr);

    if (root->expr == VAR)
        PrintVar (root, output_ptr);

    PrintNodeInGraphDump (output_ptr, root, root->left, "f0");
    PrintNodeInGraphDump (output_ptr, root, root->right, "f1");

    fprintf (output_ptr, "}");
}

void PrintNodeInGraphDump (FILE* output_ptr, node_t* node, node_t* child,
                    const char* link_for_arrow)
{
    assert (output_ptr != NULL);
    assert (node != NULL);

    if (child == NULL)
        return;

    if (child->expr == NUM)
        PrintNum (child, output_ptr);

    if (child->expr == FUNC)
        PrintFunc (child, output_ptr);

    if (child->expr == OP)
        PrintOp (child, output_ptr);

    if (child->expr == VAR)
        PrintVar (child, output_ptr);

    fprintf (output_ptr,
            "\t node%p:%s -> node%p;\n", node, link_for_arrow, child);

    PrintNodeInGraphDump (output_ptr, child, child->left, "<f0>");
    PrintNodeInGraphDump (output_ptr, child, child->right, "<f1>");
}

void PrintNum (node_t* node, FILE* output_ptr)
{
    assert (node != NULL);
    assert (output_ptr != NULL);

    fprintf (output_ptr,
            "\nnode%p [label = <                                               \n"
            "\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">   \n"
            "\t<TR> <TD COLSPAN = \"2\" BGCOLOR = \"lightblue\"> %s </TD> </TR>\n"
            "\t<TR> <TD COLSPAN = \"2\"> %p </TD> </TR>                        \n"
            "\t<TR> <TD COLSPAN = \"2\"> %lg </TD> </TR>                       \n"
            "\t<TR> <TD PORT = \"f0\"> %p </TD>                                \n"
            "\t<TD PORT = \"f1\"> %p </TD> </TR>                               \n"
            "\t</TABLE>                                                        \n"
            "\t>];                                                             \n",
            node, "NUM_type", node, node->data.num, node->left, node->right);
}

void PrintFunc (node_t* node, FILE* output_ptr)
{
    assert (node != NULL);
    assert (output_ptr != NULL);

    fprintf (output_ptr,
            "\nnode%p [label = <                                            \n"
            "\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n"
            "\t<TR> <TD COLSPAN = \"2\" BGCOLOR = \"plum\"> %s </TD> </TR>  \n"
            "\t<TR> <TD COLSPAN = \"2\"> %p </TD> </TR>                     \n"
            "\t<TR> <TD COLSPAN = \"2\"> %s </TD> </TR>                     \n"
            "\t<TR> <TD PORT = \"f0\"> %p </TD>                             \n"
            "\t<TD PORT = \"f1\"> %p </TD> </TR>                            \n"
            "\t</TABLE>                                                     \n"
            "\t>];                                                          \n",
            node, "FUNC_type", node, list_of_func[node->data.func].name,
            node->left, node->right);
}

void PrintOp (node_t* node, FILE* output_ptr)
{
    assert (node != NULL);
    assert (output_ptr != NULL);

    fprintf (output_ptr,
            "\nnode%p [label = <                                               \n"
            "\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">   \n"
            "\t<TR> <TD COLSPAN = \"2\" BGCOLOR = \"palegreen\"> %s </TD> </TR>\n"
            "\t<TR> <TD COLSPAN = \"2\"> %p </TD> </TR>                        \n"
            "\t<TR> <TD COLSPAN = \"2\"> %c </TD> </TR>                        \n"
            "\t<TR> <TD PORT = \"f0\"> %p </TD>                                \n"
            "\t<TD PORT = \"f1\"> %p </TD> </TR>                               \n"
            "\t</TABLE>                                                        \n"
            "\t>];                                                             \n",
            node, "OP_type", node, list_of_op[node->data.op].name,
            node->left, node->right);
}

void PrintVar (node_t* node, FILE* output_ptr)
{
    assert (node != NULL);
    assert (output_ptr != NULL);

    fprintf (output_ptr,
            "\nnode%p [label = <                                               \n"
            "\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">   \n"
            "\t<TR> <TD COLSPAN = \"2\" BGCOLOR = \"lightpink\"> %s </TD> </TR>\n"
            "\t<TR> <TD COLSPAN = \"2\"> %p </TD> </TR>                        \n"
            "\t<TR> <TD COLSPAN = \"2\"> %s </TD> </TR>                        \n"
            "\t<TR> <TD PORT = \"f0\"> %p </TD>                                \n"
            "\t<TD PORT = \"f1\"> %p </TD> </TR>                               \n"
            "\t</TABLE>                                                        \n"
            "\t>];                                                             \n",
            node, "VAR_type", node, node->data.var, node->left, node->right);
}

void ClearDump (const char* name_of_file)
{
    assert (name_of_file != NULL);

    FILE* output_ptr = fopen (name_of_file, "w");

    fclose (output_ptr);
}
