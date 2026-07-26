#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../headers/dump.h"

const int SIZE_OF_LINE = 65;
const int SIZE_OF_NUM = 5;
const int SIZE_OF_OP = 1;
const int SIZE_OF_FUNC = 6;

void TexNode    (FILE* output_ptr, node_t* node, int* line_size);

void TexNum     (FILE* output_ptr, node_t* node);
void TexVar     (FILE* output_ptr, node_t* node);
void TexFunc    (FILE* output_ptr, node_t* node, int* line_size);
void TexOp      (FILE* output_ptr, node_t* node, int* line_size);
void TexDiffVar (FILE* output_ptr, node_t* node);

bool TexIfPow   (FILE* output_ptr, node_t* node, int* line_size);
bool TexIfMul   (FILE* output_ptr, node_t* node, int* line_size);
bool TexIfDiv   (FILE* output_ptr, node_t* node, int* line_size);

void RunTexDump (const char* name_of_file, tree_t* tree)
{
    assert (tree);
    assert (name_of_file);

    FILE* tex_output_ptr = fopen (name_of_file, "a");
    assert (tex_output_ptr);

    ClearDump (name_of_file);

    fprintf (tex_output_ptr, "\\documentclass[12pt, a4paper]{article}\n"
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

void AddTexLine (const char* name_of_file, node_t* root, const char* phrase)
{
    assert (name_of_file);
    assert (phrase);
    assert (root);

    FILE* tex_output_ptr = fopen (name_of_file, "a");
    assert (tex_output_ptr);

    fprintf (tex_output_ptr,
            "\\textbf{%s}\n"
            "\\begin{align*}\n",
            phrase);

    int line_size = 0;

    TexNode (tex_output_ptr, root, &line_size);

    fprintf (tex_output_ptr,
            "\\end{align*}\n\n");

    fclose (tex_output_ptr);
}

void FinishTex (const char* name_of_file)
{
    assert (name_of_file);

    FILE* tex_output_ptr = fopen (name_of_file, "a");
    assert (tex_output_ptr);

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
    assert (output_ptr);
    assert (node);
    assert (line_size);

    if (*line_size >= SIZE_OF_LINE / 2) {
        // fprintf (output_ptr, " \\\\ \n");
        *line_size = 0;
    }

    switch (node->expr) {
        case FUNC:
            if (*line_size >= SIZE_OF_LINE - SIZE_OF_FUNC) {
                // fprintf (output_ptr, " \\\\ \n");
                *line_size = 0;
            }

            *line_size += SIZE_OF_FUNC;
            TexFunc (output_ptr, node, line_size);

            break;

        case NUM:
            *line_size += SIZE_OF_NUM;
            TexNum (output_ptr, node);

            break;

        case OP:
            *line_size += SIZE_OF_OP;
            TexOp (output_ptr, node, line_size);

            break;

        case VAR:
            *line_size += strlen (node->data.var.name);
            TexVar (output_ptr, node);

            break;

        case DIFF_VAR:
            *line_size += strlen (node->data.var.name) + 1;
            TexDiffVar (output_ptr, node);

            break;

        default:
            assert (0);
    }
}

void TexNum (FILE* output_ptr, node_t* node)
{
    assert (output_ptr);
    assert (node);

    fprintf (output_ptr, "%lg", node->data.num);
}

void TexVar (FILE* output_ptr, node_t* node)
{
    assert (output_ptr);
    assert (node);

    fprintf (output_ptr, "%s", node->data.var.name);
}

void TexDiffVar (FILE* output_ptr, node_t* node)
{
    assert (output_ptr);
    assert (node);

    if (node->data.var.differential_order == 1)
        fprintf (output_ptr, "\\partial %s", node->data.var.name);

    else
        fprintf (output_ptr, "\\partial^%d %s",
                node->data.var.differential_order, node->data.var.name);
}

void TexFunc (FILE* output_ptr, node_t* node, int* line_size)
{
    assert (output_ptr);
    assert (node);
    assert (line_size);

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
    assert (output_ptr);
    assert (node);
    assert (line_size);

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

    if (R->expr != OP && R->expr != FUNC)
        need_brac_right = false;

    if (L->expr != OP && L->expr != FUNC)
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
