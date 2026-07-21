#include <stdio.h>
#include <assert.h>

#include "../headers/dump.h"

void PrintNum     (node_t* node, FILE* output_ptr);
void PrintFunc    (node_t* node, FILE* output_ptr);
void PrintOp      (node_t* node, FILE* output_ptr);
void PrintVar     (node_t* node, FILE* output_ptr);
void PrintDiffVar (node_t* node, FILE* output_ptr);

void RunGraphDump (tree_t* tree, const char* name_of_file,
                   const char* cmd_to_launch_graph_dump)
{
    assert (tree);
    assert (name_of_file);
    assert (cmd_to_launch_graph_dump);

    ClearDump (name_of_file);

    FILE* output_ptr = fopen (name_of_file, "a");
    assert (output_ptr);

    PrintTreeInGraphDump (tree->root, output_ptr);

    fclose (output_ptr);

    system (cmd_to_launch_graph_dump);
}

void PrintTreeInGraphDump (node_t* root, FILE* output_ptr)
{
    assert (root);
    assert (output_ptr);

    fprintf (output_ptr, "digraph demo { \n"
            "\t rankdir = TB; \n"
            "\t splines = ortho;\n"
            "\t node [shape = plaintext];\n"
            "\n");

    switch (root->expr) {
        case NUM:
            PrintNum (root, output_ptr);

            break;

        case FUNC:
            PrintFunc (root, output_ptr);

            break;

        case OP:
            PrintOp (root, output_ptr);

            break;

        case VAR:
            PrintVar (root, output_ptr);

            break;

        case DIFF_VAR:
            PrintDiffVar (root, output_ptr);

            break;

        default:
            assert (0);
    }

    PrintNodeInGraphDump (output_ptr, root, root->left, "f0");
    PrintNodeInGraphDump (output_ptr, root, root->right, "f1");

    fprintf (output_ptr, "}");
}

void PrintNodeInGraphDump (FILE* output_ptr, node_t* node, node_t* child,
                    const char* link_for_arrow)
{
    assert (output_ptr);
    assert (node);

    if (child == NULL)
        return;

    switch (child->expr) {
        case NUM:
            PrintNum (child, output_ptr);

            break;

        case FUNC:
            PrintFunc (child, output_ptr);

            break;

        case OP:
            PrintOp (child, output_ptr);

            break;

        case VAR:
            PrintVar (child, output_ptr);

            break;

        case DIFF_VAR:
            PrintDiffVar (child, output_ptr);

            break;

        default:
            assert (0);
    }

    fprintf (output_ptr, "\t node%p:%s -> node%p;\n",
            node, link_for_arrow, child);

    PrintNodeInGraphDump (output_ptr, child, child->left, "<f0>");
    PrintNodeInGraphDump (output_ptr, child, child->right, "<f1>");
}

void PrintNum (node_t* node, FILE* output_ptr)
{
    assert (node);
    assert (output_ptr);

    fprintf (output_ptr,
            "\nnode%p [label = <                                               \n"
            "\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">   \n"
            "\t<TR> <TD COLSPAN = \"2\" BGCOLOR = \"lightblue\"> %s </TD> </TR>\n"
            "\t<TR> <TD COLSPAN = \"2\"> his parent: %p </TD> </TR>            \n"
            "\t<TR> <TD COLSPAN = \"2\"> he: %p </TD> </TR>                    \n"
            "\t<TR> <TD COLSPAN = \"2\"> %lg </TD> </TR>                       \n"
            "\t<TR> <TD PORT = \"f0\"> %p </TD>                                \n"
            "\t<TD PORT = \"f1\"> %p </TD> </TR>                               \n"
            "\t</TABLE>                                                        \n"
            "\t>];                                                             \n",
            node, "NUM_type", node->parent, node, node->data.num, L, R);
}

void PrintFunc (node_t* node, FILE* output_ptr)
{
    assert (node);
    assert (output_ptr);

    fprintf (output_ptr,
            "\nnode%p [label = <                                            \n"
            "\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n"
            "\t<TR> <TD COLSPAN = \"2\" BGCOLOR = \"plum\"> %s </TD> </TR>  \n"
            "\t<TR> <TD COLSPAN = \"2\"> his parent: %p </TD> </TR>         \n"
            "\t<TR> <TD COLSPAN = \"2\"> %p </TD> </TR>                     \n"
            "\t<TR> <TD COLSPAN = \"2\"> %s </TD> </TR>                     \n"
            "\t<TR> <TD PORT = \"f0\"> %p </TD>                             \n"
            "\t<TD PORT = \"f1\"> %p </TD> </TR>                            \n"
            "\t</TABLE>                                                     \n"
            "\t>];                                                          \n",
            node, "FUNC_type", node->parent, node, list_of_func[node->data.func].name, L, R);
}

void PrintOp (node_t* node, FILE* output_ptr)
{
    assert (node);
    assert (output_ptr);

    fprintf (output_ptr,
            "\nnode%p [label = <                                               \n"
            "\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">   \n"
            "\t<TR> <TD COLSPAN = \"2\" BGCOLOR = \"palegreen\"> %s </TD> </TR>\n"
            "\t<TR> <TD COLSPAN = \"2\"> his parent: %p </TD> </TR>            \n"
            "\t<TR> <TD COLSPAN = \"2\"> %p </TD> </TR>                        \n"
            "\t<TR> <TD COLSPAN = \"2\"> %c </TD> </TR>                        \n"
            "\t<TR> <TD PORT = \"f0\"> %p </TD>                                \n"
            "\t<TD PORT = \"f1\"> %p </TD> </TR>                               \n"
            "\t</TABLE>                                                        \n"
            "\t>];                                                             \n",
            node, "OP_type", node->parent, node, list_of_op[node->data.op].name, L, R);
}

void PrintVar (node_t* node, FILE* output_ptr)
{
    assert (node);
    assert (output_ptr);

    fprintf (output_ptr,
            "\nnode%p [label = <                                               \n"
            "\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">   \n"
            "\t<TR> <TD COLSPAN = \"2\" BGCOLOR = \"lightpink\"> %s </TD> </TR>\n"
            "\t<TR> <TD COLSPAN = \"2\"> his parent: %p </TD> </TR>            \n"
            "\t<TR> <TD COLSPAN = \"2\"> %p </TD> </TR>                        \n"
            "\t<TR> <TD COLSPAN = \"2\"> %s </TD> </TR>                        \n"
            "\t<TR> <TD PORT = \"f0\"> %p </TD>                                \n"
            "\t<TD PORT = \"f1\"> %p </TD> </TR>                               \n"
            "\t</TABLE>                                                        \n"
            "\t>];                                                             \n",
            node, "VAR_type", node->parent, node, node->data.var.name, L, R);
}

void PrintDiffVar (node_t* node, FILE* output_ptr)
{
    assert (node);
    assert (output_ptr);

    fprintf (output_ptr,
            "\nnode%p [label = <                                               \n"
            "\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">   \n"
            "\t<TR> <TD COLSPAN = \"2\" BGCOLOR = \"lightpink\"> %s </TD> </TR>\n"
            "\t<TR> <TD COLSPAN = \"2\"> his parent: %p </TD> </TR>            \n"
            "\t<TR> <TD COLSPAN = \"2\"> %p </TD> </TR>                        \n"
            "\t<TR> <TD COLSPAN = \"2\"> d %s </TD> </TR>                      \n"
            "\t<TR> <TD PORT = \"f0\"> %p </TD>                                \n"
            "\t<TD PORT = \"f1\"> %p </TD> </TR>                               \n"
            "\t</TABLE>                                                        \n"
            "\t>];                                                             \n",
            node, "VAR_type", node->parent, node, node->data.var.name, L, R);
}
