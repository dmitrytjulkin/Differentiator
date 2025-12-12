#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"

const int SIZE_OF_LINE = 65;
const int SIZE_OF_NUM = 5;
const int SIZE_OF_OP = 1;
const int SIZE_OF_FUNC = 6;

void TexNode (FILE* output_ptr, node_t* node, int* line_size);

void TexNum  (FILE* output_ptr, node_t* node);
void TexVar  (FILE* output_ptr, node_t* node);
void TexFunc (FILE* output_ptr, node_t* node, int* line_size);
void TexOp   (FILE* output_ptr, node_t* node, int* line_size);

bool TexIfPow (FILE* output_ptr, node_t* node, int* line_size);
bool TexIfMul (FILE* output_ptr, node_t* node, int* line_size);
bool TexIfDiv (FILE* output_ptr, node_t* node, int* line_size);


void RunTexDump (const char* name_of_file, tree_t* tree)
{
    assert (tree != NULL);
    assert (name_of_file != NULL);

    FILE* tex_output_ptr = fopen (name_of_file, "a");
    assert (tex_output_ptr != NULL);

    ClearDump ("tree.tex");

    fprintf (tex_output_ptr, "\\documentclass[12pt, a4paper]{article}"
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
            "\\begin{equation}"
            "\n\\begin{split}\n");

    int line_size = 0;

    TexNode (tex_output_ptr, tree->root, &line_size);

    fprintf (tex_output_ptr,
            "\n\\end{split}\n"
            "\\end{equation}\n"
            "\n");

    fclose (tex_output_ptr);
}

void AddTexLine (const char* name_of_file, node_t* node, const char* phrase)
{
    assert (name_of_file != NULL);
    assert (node != NULL);

    FILE* tex_output_ptr = fopen (name_of_file, "a");
    assert (tex_output_ptr != NULL);

    node_t* root = FindRoot (node);

    fprintf (tex_output_ptr,
            "\\textbf{%s}\n"
            "\\begin{equation}\n"
            "\\begin{split}",
            phrase);

    int line_size = 0;

    TexNode (tex_output_ptr, root, &line_size);

    fprintf (tex_output_ptr,
            "\n\\end{split}\n"
            "\\end{equation}\n\n");

    fclose (tex_output_ptr);
}

void FinishTex (const char* name_of_file)
{
    assert (name_of_file != NULL);

    FILE* tex_output_ptr = fopen (name_of_file, "a");
    assert (tex_output_ptr != NULL);

    fprintf (tex_output_ptr,
            "\\textbf{Дальнейшие преобразования, оставим читателю "
            "в качестве самостоятельного упражнения}.\n"
            "\n"
            "Approved by \"Кафедра вышмата\"\n"
            "\\end{document}\n");

    fclose (tex_output_ptr);
}

void TexNode (FILE* output_ptr, node_t* node, int* line_size)
{
    assert (output_ptr != NULL);
    assert (node != NULL);

    if (*line_size >= SIZE_OF_LINE) {
        fprintf (output_ptr, " \\\\ \n");

        *line_size = 0;
    }

    if (node->expr == FUNC && *line_size >= SIZE_OF_LINE - SIZE_OF_FUNC) {
        fprintf (output_ptr, " \\\\ \n");

        *line_size = 0;
    }

    if (node->expr == NUM) {
        *line_size += SIZE_OF_NUM;

        TexNum (output_ptr, node);
    }

    if (node->expr == OP) {
        *line_size += SIZE_OF_OP;

        TexOp (output_ptr, node, line_size);
    }

    if (node->expr == FUNC) {
        *line_size += SIZE_OF_FUNC;

        TexFunc (output_ptr, node, line_size);
    }

    if (node->expr == VAR) {
        *line_size += strlen (node->data.var);

        TexVar (output_ptr, node);
    }
}

void TexNum (FILE* output_ptr, node_t* node)
{
    fprintf (output_ptr, "%lg", node->data.num);
}

void TexVar (FILE* output_ptr, node_t* node)
{
    fprintf (output_ptr, "%s", node->data.var);
}

void TexFunc (FILE* output_ptr, node_t* node, int* line_size)
{
    if (node->expr == FUNC) {
        if (node->data.func == SQRT) {
            fprintf (output_ptr, "\\%s{", list_of_func[node->data.func].name);

            TexNode (output_ptr, R, line_size);

            fprintf (output_ptr, "}");

            return;
        }

        fprintf (output_ptr, "\\%s(", list_of_func[node->data.func].name);

        TexNode (output_ptr, R, line_size);

        fprintf (output_ptr, ")");

        return;
    }
}

void TexOp (FILE* output_ptr, node_t* node, int* line_size)
{
    if (node->expr == OP) {
        if (TexIfPow (output_ptr, node, line_size)) return;

        if (TexIfDiv (output_ptr, node, line_size)) return;

        if (TexIfMul (output_ptr, node, line_size)) return;

        TexNode (output_ptr, L, line_size);

        fprintf (output_ptr, " %c ", list_of_op[node->data.op].name);

        TexNode (output_ptr, R, line_size);
    }
}

bool TexIfDiv (FILE* output_ptr, node_t* node, int* line_size)
{
    if (node->data.op != DIV)
        return false;

    fprintf (output_ptr, " \\frac {");

    TexNode (output_ptr, L, line_size);

    fprintf (output_ptr, "} {");

    TexNode (output_ptr, R, line_size);

    fprintf (output_ptr, "} ");

    return true;
}

bool TexIfMul (FILE* output_ptr, node_t* node, int* line_size)
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

    TexNode (output_ptr, L, line_size);

    if (need_brac_left && need_brac_right)
        fprintf (output_ptr, ") \\cdot (");

    else if (need_brac_left && !need_brac_right)
        fprintf (output_ptr, ") \\cdot ");

    else if (!need_brac_left && need_brac_right)
        fprintf (output_ptr, "\\cdot (");

    else
        fprintf (output_ptr, " \\cdot ");

    TexNode (output_ptr, R, line_size);

    if (need_brac_right)
        fprintf (output_ptr, ")");

    return true;
}

bool TexIfPow (FILE* output_ptr, node_t* node, int* line_size)
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

    TexNode (output_ptr, L, line_size);

    if (need_brac_right && need_brac_left)
        fprintf (output_ptr, ") ^ {");

    else if (need_brac_right && !need_brac_left)
        fprintf (output_ptr, " ^ {");

    else if (!need_brac_right && need_brac_left)
        fprintf (output_ptr, ") ^ ");

    else
        fprintf (output_ptr, " ^ ");

    TexNode (output_ptr, R, line_size);

    if (need_brac_right)
        fprintf (output_ptr, "}");

    return true;
}
