#include <stdio.h>
#include <assert.h>

#include "tree.h"

void TexNode (FILE* output_ptr, node_t* node);

void TexNum  (FILE* output_ptr, node_t* node);
void TexFunc (FILE* output_ptr, node_t* node);
void TexVar  (FILE* output_ptr, node_t* node);
void TexOp   (FILE* output_ptr, node_t* node);

bool TexIfPow (FILE* output_ptr, node_t* node);
bool TexIfMul (FILE* output_ptr, node_t* node);
bool TexIfDiv (FILE* output_ptr, node_t* node);

// TODO x^x^x - it works if x^(x^x)

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
            "\n\\[\n");

    TexNode (output_ptr, tree->root);

    fprintf (output_ptr,
            "\n\\]\n"
            "\n"
            "\\textbf{Получим производную, "
            "приведенную элементарными преобразованиями:}\n"
            "\n"
            "\\[\n");

    TexNode (output_ptr, der_tree->root);

    fprintf (output_ptr,
            "\n\\]"
            "\\textbf{Дальнейшие преобразования, оставим читателю "
            "в качестве самостоятельного упражнения}\n"
            "\n"
            "\\end{document}\n");

    fclose (output_ptr);
}

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
        fprintf (output_ptr, "\\%s(", node->data.op);

        TexNode (output_ptr, node->right);

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

        TexNode (output_ptr, node->left);

        fprintf (output_ptr, " %s ", node->data.op);

        TexNode (output_ptr, node->right);
    }
}

bool TexIfDiv (FILE* output_ptr, node_t* node)
{
    if (node->data.op[0] != '/')
        return false;


    fprintf (output_ptr, " \\frac {");

    TexNode (output_ptr, node->left);

    fprintf (output_ptr, "} {");

    TexNode (output_ptr, node->right);

    fprintf (output_ptr, "} ");

    return true;
}

bool TexIfMul (FILE* output_ptr, node_t* node)
{
    if (node->data.op[0] != '*')
        return false;

    int need_brac_left = true;
    int need_brac_right = true;

    if (node->right->expr != OP &&
        node->right->data.op[0] != '+' && node->right->data.op[0] != '-')
        need_brac_right = false;

    if (node->left->expr != OP &&
        node->right->data.op[0] != '+' && node->right->data.op[0] != '-')
        need_brac_left = false;

    if (need_brac_left)
        fprintf (output_ptr, "(");

    TexNode (output_ptr, node->left);

    if (need_brac_left && need_brac_right)
        fprintf (output_ptr, ") \\cdot (");

    else if (need_brac_left && !need_brac_right)
        fprintf (output_ptr, ") \\cdot ");

    else if (!need_brac_left && need_brac_right)
        fprintf (output_ptr, "\\cdot (");

    else
        fprintf (output_ptr, " \\cdot ");

    TexNode (output_ptr, node->right);

    if (need_brac_right)
        fprintf (output_ptr, ")");

    return true;
}

bool TexIfPow (FILE* output_ptr, node_t* node)
{
    if (node->data.op[0] != '^')
        return false;

    int need_brac_left = true;
    int need_brac_right = true;

    if (node->right->expr != OP)
        need_brac_right = false;

    if (node->left->expr != OP)
        need_brac_left = false;

    if (need_brac_left)
        fprintf (output_ptr, "(");

    TexNode (output_ptr, node->left);

    if (need_brac_right && need_brac_left)
        fprintf (output_ptr, ") ^ {");

    else if (need_brac_right && !need_brac_left)
        fprintf (output_ptr, " ^ {");

    else if (!need_brac_right && need_brac_left)
        fprintf (output_ptr, ") ^ ");

    else
        fprintf (output_ptr, " ^ ");

    TexNode (output_ptr, node->right);

    if (need_brac_right)
        fprintf (output_ptr, "}");

    return true;
}
