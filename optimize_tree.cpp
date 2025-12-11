#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "tree.h"


node_t* ChooseOptimizingAlgorithm (node_t* node, bool* is_tree_changed);

node_t* OptimizeAdd (node_t* node, bool* is_tree_changed);
node_t* OptimizeSub (node_t* node, bool* is_tree_changed);
node_t* OptimizeMul (node_t* node, bool* is_tree_changed);
node_t* OptimizeDiv (node_t* node, bool* is_tree_changed);
node_t* OptimizePow (node_t* node, bool* is_tree_changed);

node_t* OptimizeAddZero (node_t* node, bool* is_tree_changed);
node_t* OptimizeSubZero (node_t* node, bool* is_tree_changed);
node_t* OptimizeMulZero (node_t* node, bool* is_tree_changed);
node_t* OptimizeDivZero (node_t* node, bool* is_tree_changed);
node_t* OptimizePowZero (node_t* node, bool* is_tree_changed);

node_t* OptimizeMulOne (node_t* node, bool* is_tree_changed);
node_t* OptimizeDivOne (node_t* node, bool* is_tree_changed);
node_t* OptimizePowOne (node_t* node, bool* is_tree_changed);

bool IsZero (double a);


node_t* Optimize (tree_t* tree)
{
    assert (tree != NULL);

    bool is_tree_changed = false;

    node_t* improved_node = ChooseOptimizingAlgorithm (tree->root, &is_tree_changed);

    while (is_tree_changed) {
        is_tree_changed = false;

        improved_node = ChooseOptimizingAlgorithm (improved_node, &is_tree_changed);
    }

    return improved_node;
}

node_t* ChooseOptimizingAlgorithm (node_t* node, bool* is_tree_changed)
{
    assert (is_tree_changed != NULL);

    if (node == NULL)
        return node;

    if (R == NULL || L == NULL)
        return node;

    L = ChooseOptimizingAlgorithm (L, is_tree_changed);
    R = ChooseOptimizingAlgorithm (R, is_tree_changed);

    node_t* new_node = InitNode ();

    if (node->expr == OP && node->data.op == ADD)
        if ((new_node = OptimizeAdd (node, is_tree_changed)) != NULL)
            return new_node;

    if (node->expr == OP && node->data.op == SUB)
        if ((new_node = OptimizeSub (node, is_tree_changed)) != NULL)
            return new_node;

    if (node->expr == OP && node->data.op == MUL)
        if ((new_node = OptimizeMul (node, is_tree_changed)) != NULL)
            return new_node;

    if (node->expr == OP && node->data.op == DIV)
        if ((new_node = OptimizeDiv (node, is_tree_changed)) != NULL)
            return new_node;

    if (node->expr == OP && node->data.op == POW)
        if ((new_node = OptimizePow (node, is_tree_changed)) != NULL)
            return new_node;

    return node;
}

node_t* OptimizeAdd (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    OPTIMIZE_IF_NUM_OP_NUM (+);

    node_t* new_node = InitNode ();

    if ((new_node = OptimizeAddZero (node, is_tree_changed)) != NULL)
        return new_node;

    return NULL;
}

node_t* OptimizeSub (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    OPTIMIZE_IF_NUM_OP_NUM (-);

    node_t* new_node = InitNode ();

    if ((new_node = OptimizeSubZero (node, is_tree_changed)) != NULL)
        return new_node;

    return NULL;
}

node_t* OptimizeMul (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    OPTIMIZE_IF_NUM_OP_NUM (*);

    node_t* new_node = InitNode ();

    if ((new_node = OptimizeMulZero (node, is_tree_changed)) != NULL)
        return new_node;

    if ((new_node = OptimizeMulOne (node, is_tree_changed)) != NULL)
        return new_node;

    return NULL;
}

node_t* OptimizeDiv (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    OPTIMIZE_IF_NUM_OP_NUM (/);

    node_t* new_node = InitNode ();

    if ((new_node = OptimizeDivZero (node, is_tree_changed)) != NULL)
        return new_node;

    if ((new_node = OptimizeDivOne (node, is_tree_changed)) != NULL)
        return new_node;

    return NULL;
}

node_t* OptimizePow (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (L->expr == NUM && R->expr == NUM) {
        node->expr = NUM;
        node->data.num = pow (DeleteNodeAndRetData (node, LEFT).num,
                              DeleteNodeAndRetData (node, RIGHT).num);

        return node;
    }

    node_t* new_node = InitNode ();

    if ((new_node = OptimizePowZero (node, is_tree_changed)) != NULL)
        return new_node;

    if ((new_node = OptimizePowOne (node, is_tree_changed)) != NULL)
        return new_node;

    return NULL;
}

node_t* OptimizeAddZero (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if ((R->expr != NUM || !IsZero (R->data.num)) &&
        (L->expr != NUM || !IsZero (L->data.num)))
        return NULL;

    *is_tree_changed = true;

    if (R->expr == NUM && IsZero (R->data.num))
        return NewNode (L->expr, L->data,
                        CopyNode (L->left), CopyNode (L->right));

    else
        return NewNode (R->expr, R->data,
                        CopyNode (R->left), CopyNode (R->right));
}

node_t* OptimizeSubZero (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (R->expr != NUM || !IsZero (R->data.num))
        return NULL;

    *is_tree_changed = true;

    return NewNode (L->expr, L->data,
                    CopyNode (L->left), CopyNode (L->right));

}

node_t* OptimizeMulZero (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if ((R->expr != NUM || !IsZero (R->data.num)) &&
        (L->expr != NUM || !IsZero (L->data.num)))
        return NULL;

    *is_tree_changed = true;

    return NewNode (NUM, {.num = 0}, NULL, NULL);
}

node_t* OptimizeDivZero (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (L->expr != NUM || !IsZero (L->data.num))
        return NULL;

    *is_tree_changed = true;

    return NewNode (NUM, {.num = 0}, NULL, NULL);
}

node_t* OptimizePowZero (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if ((R->expr != NUM || !IsZero (R->data.num)) &&
        (L->expr != NUM || !IsZero (L->data.num)))
        return NULL;

    *is_tree_changed = true;

    if (R->expr == NUM && IsZero (R->data.num))
        return NewNode (NUM, {.num = 1}, NULL, NULL);

    else
        return NewNode (NUM, {.num = 0}, NULL, NULL);
}


node_t* OptimizeMulOne (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if ((R->expr != NUM || !IsZero (R->data.num - 1)) &&
        (L->expr != NUM || !IsZero (L->data.num - 1)))
        return NULL;

    *is_tree_changed = true;

    if (R->expr == NUM && IsZero (R->data.num - 1))
        return NewNode (L->expr, L->data,
                        CopyNode (L->left), CopyNode (L->right));

    else
        return NewNode (R->expr, R->data,
                        CopyNode (R->left), CopyNode (R->right));
}

node_t* OptimizeDivOne (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if (R->expr != NUM || !IsZero (R->data.num - 1))
        return NULL;

    *is_tree_changed = true;

    return NewNode (L->expr, L->data,
                    CopyNode (L->left), CopyNode (L->right));
}

node_t* OptimizePowOne (node_t* node, bool* is_tree_changed)
{
    assert (node != NULL);
    assert (is_tree_changed != NULL);

    if ((R->expr != NUM || !IsZero (R->data.num - 1)) &&
        (L->expr != NUM || !IsZero (L->data.num - 1)))
        return NULL;

    *is_tree_changed = true;

    if (L->expr == NUM && IsZero (L->data.num - 1))
        return NewNode (NUM, {.num = 1}, NULL, NULL);

    else
        return NewNode (L->expr, L->data,
                        CopyNode (L->left), CopyNode (L->right));
}

bool IsZero (double a)
{
    if (fabs(a) < 1e-4)
        return 1;

    return 0;
}
