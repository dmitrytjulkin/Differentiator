#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "tree.h"


node_t* ChooseOptimizingAlgorithm (node_t* node, int* is_tree_changed);

node_t* OptimizeOpBetweenNum (node_t* node, int* is_tree_changed);
node_t* OptimizeIfNodeIsZero (node_t* node, int* is_tree_changed);
node_t* OptimizeIfNodeIsOne  (node_t* node, int* is_tree_changed);

node_t* OptimizeAddZero (node_t* node, int* is_tree_changed);
node_t* OptimizeSubZero (node_t* node, int* is_tree_changed);
node_t* OptimizeMulZero (node_t* node, int* is_tree_changed);
node_t* OptimizeDivZero (node_t* node, int* is_tree_changed);
node_t* OptimizePowZero (node_t* node, int* is_tree_changed);

node_t* OptimizeMulOne (node_t* node, int* is_tree_changed);
node_t* OptimizeDivOne (node_t* node, int* is_tree_changed);
node_t* OptimizePowOne (node_t* node, int* is_tree_changed);


node_t* Optimize (tree_t* tree)
{
    assert (tree != NULL);

    int is_tree_changed = false;

    node_t* improved_node = InitNode();

    improved_node = ChooseOptimizingAlgorithm (tree->root, &is_tree_changed);

    while (is_tree_changed) {
        is_tree_changed = false;

        improved_node = ChooseOptimizingAlgorithm (improved_node, &is_tree_changed);
    }

    return improved_node;
}

node_t* ChooseOptimizingAlgorithm (node_t* node, int* is_tree_changed)
{
    assert (is_tree_changed != NULL);

    if (node == NULL)
        return node;

    if (node->right == NULL)
        return node;

    if (node->left == NULL)
        return node;

    node->left = ChooseOptimizingAlgorithm (node->left, is_tree_changed);
    node->right = ChooseOptimizingAlgorithm (node->right, is_tree_changed);

    if (node->expr == OP && node->left->expr == NUM && node->right->expr == NUM)
        return OptimizeOpBetweenNum (node, is_tree_changed);

    if ((node->left->expr == NUM && IsZero (node->left->data.num)) ||
        (node->right->expr == NUM && IsZero (node->right->data.num)))
        return OptimizeIfNodeIsZero (node, is_tree_changed);

    if ((node->left->expr == NUM && IsZero(node->left->data.num - 1)) ||
        (node->right->expr == NUM && IsZero(node->right->data.num - 1)))
        return OptimizeIfNodeIsOne (node, is_tree_changed);

    return node;
}

node_t* OptimizeOpBetweenNum (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);

    *is_tree_changed = true;

    OPTIMIZE_NUM_OP_NUM (ADD, +);

    OPTIMIZE_NUM_OP_NUM (SUB, -);

    OPTIMIZE_NUM_OP_NUM (MUL, *);

    OPTIMIZE_NUM_OP_NUM (DIV, /);

    if (node->data.op == POW) {
        node->expr = NUM;
        node->data.num = pow (DeleteNodeAndRetData (node, LEFT).num,
                              DeleteNodeAndRetData (node, RIGHT).num);

        return node;
    }

    return node;
}

node_t* OptimizeIfNodeIsZero (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (node->left != NULL);
    assert (node->right != NULL);
    assert (is_tree_changed != NULL);

    node_t* new_node = InitNode ();

    SKIP_OR_DO_AND_RETURN (OptimizeAddZero (node, is_tree_changed));

    SKIP_OR_DO_AND_RETURN (OptimizeSubZero (node, is_tree_changed));

    SKIP_OR_DO_AND_RETURN (OptimizeMulZero (node, is_tree_changed));

    SKIP_OR_DO_AND_RETURN (OptimizeDivZero (node, is_tree_changed));

    SKIP_OR_DO_AND_RETURN (OptimizePowZero (node, is_tree_changed));

    return node;
}

node_t* OptimizeIfNodeIsOne (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (node->left != NULL);
    assert (node->right != NULL);
    assert (is_tree_changed != NULL);

    node_t* new_node = InitNode ();

    SKIP_OR_DO_AND_RETURN (OptimizeMulOne (node, is_tree_changed));

    SKIP_OR_DO_AND_RETURN (OptimizeDivOne (node, is_tree_changed));

    SKIP_OR_DO_AND_RETURN (OptimizePowOne (node, is_tree_changed));

    return node;
}

node_t* OptimizeAddZero (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (node->data.op == ADD) {
        *is_tree_changed = true;

        if (node->right->expr == NUM && IsZero (node->right->data.num))
            return NewNode (node->left->expr, node->left->data,
                            c (node->left->left), c (node->left->right));

        else
            return NewNode (node->right->expr, node->right->data,
                            c (node->right->left), c (node->right->right));

    } else
        return NULL;
}

node_t* OptimizeSubZero (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (node->data.op == SUB && node->right->expr == NUM &&
        IsZero (node->right->data.num)) {
        *is_tree_changed = true;

        return NewNode (node->left->expr, node->left->data,
                        c (node->left->left), c (node->left->right));

    } else
        return NULL;
}

node_t* OptimizeMulZero (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (node->data.op == MUL) {
        *is_tree_changed = true;

        return NewNode (NUM, {.num = 0}, NULL, NULL);

    } else
        return NULL;
}

node_t* OptimizeDivZero (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (node->data.op == DIV && node->left->expr == NUM
        && IsZero (node->left->data.num)) {
        *is_tree_changed = true;

        return NewNode (NUM, {.num = 0}, NULL, NULL);

    } else
        return NULL;
}

node_t* OptimizePowZero (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (node->data.op == POW) {
        *is_tree_changed = true;

        if (node->right->expr == NUM && IsZero (node->right->data.num))
            return NewNode (NUM, {.num = 1}, NULL, NULL);

        else
            return NewNode (NUM, {.num = 0}, NULL, NULL);

    } else
        return NULL;
}


node_t* OptimizeMulOne (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (node->data.op == MUL) {
        *is_tree_changed = true;

        if (node->right->expr == NUM && IsZero (node->right->data.num - 1))
            return NewNode (node->left->expr, node->left->data,
                            c (node->left->left), c (node->left->right));

        else
            return NewNode (node->right->expr, node->right->data,
                            c (node->right->left), c (node->right->right));

    } else
        return NULL;
}

node_t* OptimizeDivOne (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (node->data.op == DIV && node->right->expr == NUM
        && IsZero (node->right->data.num - 1)){
        *is_tree_changed = true;

        return NewNode (node->left->expr, node->left->data,
                        c (node->left->left), c (node->left->right));

    } else
        return NULL;
}

node_t* OptimizePowOne (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (node->data.op == POW) {
        *is_tree_changed = true;

        if (node->left->expr == NUM && IsZero (node->left->data.num - 1))
            return NewNode (NUM, {.num = 1}, NULL, NULL);

        else
            return NewNode (node->left->expr, node->left->data,
                            c (node->left->left), c (node->left->right));

    } else
        return NULL;
}
