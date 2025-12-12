#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"


void TexNode (FILE* output_ptr, node_t* node);

void TexNum  (FILE* output_ptr, node_t* node);
void TexFunc (FILE* output_ptr, node_t* node);
void TexVar  (FILE* output_ptr, node_t* node);
void TexOp   (FILE* output_ptr, node_t* node);

bool TexIfPow (FILE* output_ptr, node_t* node);
bool TexIfMul (FILE* output_ptr, node_t* node);
bool TexIfDiv (FILE* output_ptr, node_t* node);


void RunTexDump (tree_t* tree, tree_t* der_tree)
{
    assert (tree != NULL);

    FILE* output_ptr = fopen ("tree.tex", "a");
    assert (output_ptr != NULL);

    ClearDump ("tree.tex");

    fprintf (output_ptr, "\\documentclass[12pt, a4paper]{article}"
            "\\usepackage[utf8]{inputenc}\n"
            "\\usepackage[T2A]{fontenc}\n"
            "\\usepackage[russian]{babel}\n"
            "\n"
            "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n"
            "\\usepackage{amssymb}\n"
            "\n"
            "\\begin{document}\n"
            "\n"
            "\\textbf{Здесь записана формула:}\n"
            "\n"
            "\n\\begin{equation}\n");

    TexNode (output_ptr, tree->root);

    fprintf (output_ptr,
            "\n\\end{equation}\n"
            "\n"
            "\\textbf{Получим производную, "
            "приведенную элементарными преобразованиями:}\n"
            "\n"
            "\\begin{equation}\n");

    TexNode (output_ptr, der_tree->root);

    fprintf (output_ptr,
            "\n\\end{equation}\n"
            "\\textbf{Дальнейшие преобразования, оставим читателю "
            "в качестве самостоятельного упражнения}\n"
            "\n"
            "\\end{document}\n");

    fclose (output_ptr);
}
//
// void AddTexLine (FILE* output_ptr, tree_t* tree)
// {
//     assert (output_ptr != NULL);
//     assert (tree != NULL);
//
//     fprintf (output_ptr,
//             "Let's move on:\n"
//             "\\begin{equation}\n");
//
//     TexNode (output_ptr, tree->root);
//
//     fprintf (output_ptr,
//             "\end{equation}\n\n");
// }
//
// void FinishTex (FILE* output_ptr)
// {
//     assert (output_ptr != NULL);
//
//     fprintf (output_ptr,
//             "\\textbf{Дальнейшие преобразования, оставим читателю "
//             "в качестве самостоятельного упражнения}\n"
//             "\n"
//             "Approved by \"Кафедра вышмата\"\n"
//             "\\end{document}\n");
//
//     fclose (output_ptr);
// }

void TexNode (FILE* output_ptr, node_t* node)
{
    assert (output_ptr != NULL);
    assert (node != NULL);

    if (node->expr == NUM)
        TexNum (output_ptr, node);

    if (node->expr == OP)
        TexOp (output_ptr, node);

    if (node->expr == FUNC)
        TexFunc (output_ptr, node);

    if (node->expr == VAR)
        TexVar (output_ptr, node);
}

void TexNum (FILE* output_ptr, node_t* node)
{
    fprintf (output_ptr, "%lg", node->data.num);
}

void TexVar (FILE* output_ptr, node_t* node)
{
    fprintf (output_ptr, "%s", node->data.var);
}

void TexFunc (FILE* output_ptr, node_t* node)
{
    if (node->expr == FUNC) {
        if (node->data.func == SQRT) {
            fprintf (output_ptr, "\\%s{", list_of_func[node->data.func].name);

            TexNode (output_ptr, R);

            fprintf (output_ptr, "}");

            return;
        }

        fprintf (output_ptr, "\\%s(", list_of_func[node->data.func].name);

        TexNode (output_ptr, R);

        fprintf (output_ptr, ")");

        return;
    }
}

void TexOp (FILE* output_ptr, node_t* node)
{
    if (node->expr == OP) {
        if (TexIfPow (output_ptr, node)) return;

        if (TexIfDiv (output_ptr, node)) return;

        if (TexIfMul (output_ptr, node)) return;

        TexNode (output_ptr, L);

        fprintf (output_ptr, " %c ", list_of_op[node->data.op].name);

        TexNode (output_ptr, R);
    }
}

bool TexIfDiv (FILE* output_ptr, node_t* node)
{
    if (node->data.op != DIV)
        return false;

    fprintf (output_ptr, " \\frac {");

    TexNode (output_ptr, L);

    fprintf (output_ptr, "} {");

    TexNode (output_ptr, R);

    fprintf (output_ptr, "} ");

    return true;
}

bool TexIfMul (FILE* output_ptr, node_t* node)
{
    if (node->data.op != MUL)
        return false;

    int need_brac_left = true;
    int need_brac_right = true;

    if (R->expr != OP || (R->data.op != ADD && R->data.op != SUB))
        need_brac_right = false;

    if (L->expr != OP || (L->data.op != ADD && L->data.op != SUB))
        need_brac_left = false;

    if (need_brac_left)
        fprintf (output_ptr, "(");

    TexNode (output_ptr, L);

    if (need_brac_left && need_brac_right)
        fprintf (output_ptr, ") \\cdot (");

    else if (need_brac_left && !need_brac_right)
        fprintf (output_ptr, ") \\cdot ");

    else if (!need_brac_left && need_brac_right)
        fprintf (output_ptr, "\\cdot (");

    else
        fprintf (output_ptr, " \\cdot ");

    TexNode (output_ptr, R);

    if (need_brac_right)
        fprintf (output_ptr, ")");

    return true;
}

bool TexIfPow (FILE* output_ptr, node_t* node)
{
    if (node->data.op != POW)
        return false;

    int need_brac_left = true;
    int need_brac_right = true;

    if (R->expr != OP)
        need_brac_right = false;

    if (L->expr != OP)
        need_brac_left = false;

    if (need_brac_left)
        fprintf (output_ptr, "(");

    TexNode (output_ptr, L);

    if (need_brac_right && need_brac_left)
        fprintf (output_ptr, ") ^ {");

    else if (need_brac_right && !need_brac_left)
        fprintf (output_ptr, " ^ {");

    else if (!need_brac_right && need_brac_left)
        fprintf (output_ptr, ") ^ ");

    else
        fprintf (output_ptr, " ^ ");

    TexNode (output_ptr, R);

    if (need_brac_right)
        fprintf (output_ptr, "}");

    return true;
}
