#include <stdio.h>
#include <assert.h>

#include "dump.h"
#include "../parse_tree/tree.h"

void FromNodeToFormula     (FILE* output_ptr, node_t* node);
void ChooseNodeTypeToPrint (FILE* output_ptr, node_t* node);

void FromTreeToFormula (const char* name_of_file, tree_t* tree)
{
    assert (name_of_file);
    assert (tree);

    FILE* output_ptr = fopen (name_of_file, "w");
    assert (output_ptr);

    if (tree->root->left != NULL)
        FromNodeToFormula (output_ptr, tree->root->left);

    ChooseNodeTypeToPrint (output_ptr, tree->root);

    if (tree->root->right)
        FromNodeToFormula (output_ptr, tree->root->right);

    fprintf (output_ptr, "\n");

    fclose (output_ptr);
}

void FromNodeToFormula (FILE* output_ptr, node_t* node)
{
    assert (output_ptr);
    assert (node);

    if (node->left != NULL)
        FromNodeToFormula (output_ptr, node->left);

    ChooseNodeTypeToPrint (output_ptr, node);

    if (node->right != NULL)
        FromNodeToFormula (output_ptr, node->right);

    if (node->expr == FUNC)
        fprintf (output_ptr, ") ");
}

void ChooseNodeTypeToPrint (FILE* output_ptr, node_t* node)
{
    assert (output_ptr);
    assert (node);

    switch (node->expr) {
        case NUM:
            fprintf (output_ptr, "%lg ", node->data.num);

            break;

        case VAR:
            fprintf (output_ptr, "%s ", node->data.var.name);

            break;

        case FUNC:
            fprintf (output_ptr, "%s ( ", list_of_func[node->data.func].name);

            break;

        case OP:
            fprintf (output_ptr, "%c ", list_of_op[node->data.op].name);

            break;

        case DIFF_VAR:
            if (node->data.var.differential_order == 1)
                fprintf (output_ptr, "(d %s) ", node->data.var.name);

            else
                fprintf (output_ptr, "(d^%d %s) ", node->data.var.differential_order,
                                                    node->data.var.name);

            break;

        default:
            assert (0);
    }
}
