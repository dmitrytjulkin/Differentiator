#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"


#define dL d(node->left)
#define dR d(node->right)
#define cL c(node->left)
#define cR c(node->right)

#define NUM_(number) NewNode (NUM, {.num = number}, NULL, NULL)

#define ADD_(left, right) NewNode (OP, {.op = "+"}, left, right)
#define SUB_(left, right) NewNode (OP, {.op = "-"}, left, right)
#define MUL_(left, right) NewNode (OP, {.op = "*"}, left, right)
#define DIV_(left, right) NewNode (OP, {.op = "/"}, left, right)
#define POW_(left, right) NewNode (OP, {.op = "^"}, left, right)

#define SQRT_(right) NewNode (FUNC, {.func = "sqrt"}, NULL, right)
#define LN_(right)   NewNode (FUNC,   {.func = "ln"}, NULL, right)
#define SIN_(right)  NewNode (FUNC,  {.func = "sin"}, NULL, right)
#define COS_(right)  NewNode (FUNC,  {.func = "cos"}, NULL, right)


node_t* dNum  (node_t* node, data_t cmd);
node_t* dVar  (node_t* node, data_t cmd);
node_t* dOp   (node_t* node);
node_t* dFunc (node_t* node);

node_t* dAddOrSub (node_t* node);
node_t* dMul      (node_t* node);
node_t* dDiv      (node_t* node);
node_t* dPow      (node_t* node);

node_t* dSqrt   (node_t* node);
node_t* dLn     (node_t* node);
node_t* dSin    (node_t* node);
node_t* dCos    (node_t* node);
node_t* dTg     (node_t* node);
node_t* dCtg    (node_t* node);
node_t* dArcsin (node_t* node);
node_t* dArccos (node_t* node);
node_t* dArctg  (node_t* node);
node_t* dArcctg (node_t* node);


node_t* d (node_t* node)
{
    if (node == NULL)
        return NULL;

    data_t cmd = {.func = ""};

    node_t* new_node = InitNode ();

    SKIP_OR_DO_AND_RETURN (dNum (node, cmd));

    SKIP_OR_DO_AND_RETURN (dVar (node, cmd));

    SKIP_OR_DO_AND_RETURN (dOp (node));

    SKIP_OR_DO_AND_RETURN (dFunc (node));

    return NULL;
}

node_t* c (node_t* node)
{
    if (node == NULL)
        return NULL;

    node_t* copy_node = InitNode ();

    copy_node->left = cL;
    copy_node->right = cR;

    copy_node->expr = node->expr;

    if (node->expr == NUM)
        copy_node->data.num = node->data.num;

    else
        strcpy (copy_node->data.func, node->data.func);

    return copy_node;
}

node_t* dNum (node_t* node, data_t cmd)
{
    assert (node != NULL);

    if (node->expr != NUM)
        return NULL;

    cmd.num = 0;

    return NewNode (NUM, cmd, NULL, NULL);
}

node_t* dVar (node_t* node, data_t cmd)
{
    assert (node != NULL);

    if (node->expr != VAR)
        return NULL;

    cmd.num = 1;

    return NewNode (NUM, (data_t) {.num = 1}, NULL, NULL);

}

node_t* dOp (node_t* node)
{
    assert (node != NULL);

    if (node->expr != OP)
        return NULL;

    node_t* new_node = InitNode ();

    SKIP_OR_DO_AND_RETURN (dAddOrSub (node));

    SKIP_OR_DO_AND_RETURN (dMul (node));

    SKIP_OR_DO_AND_RETURN (dDiv (node));

    SKIP_OR_DO_AND_RETURN (dPow (node));

    printf ("This type of op doesn't exist: %s\n", node->data.op);
    assert (0);
}

node_t* dFunc (node_t* node)
{
    assert (node != NULL);

    if (node->expr != FUNC)
        return NULL;

    node_t* new_node = InitNode ();

    SKIP_OR_DO_AND_RETURN (dSqrt   (node));

    SKIP_OR_DO_AND_RETURN (dLn     (node));

    SKIP_OR_DO_AND_RETURN (dSin    (node));

    SKIP_OR_DO_AND_RETURN (dCos    (node));

    SKIP_OR_DO_AND_RETURN (dTg     (node));

    SKIP_OR_DO_AND_RETURN (dCtg    (node));

    SKIP_OR_DO_AND_RETURN (dArcsin (node));

    printf ("This type of func doesn't exist: %s\n", node->data.func);
    assert (0);
}

node_t* dAddOrSub (node_t* node)
{
    assert (node != NULL);

    if (node->data.op[0] != '+' && node->data.op[0] != '-')
        return NULL;

    return NewNode (OP, node->data, dL, dR);
}

node_t* dMul (node_t* node)
{
    assert (node != NULL);

    if (node->data.op[0] != '*')
        return NULL;

    return ADD_ (MUL_ (dL, cR), MUL_ (cL, dR));
}

node_t* dDiv (node_t* node)
{
    assert (node != NULL);

    if (node->data.op[0] != '/')
        return NULL;

    node_t* numer = SUB_ (MUL_ (dL, cR), MUL_ (cL, dR));
    node_t* denominator = POW_ (cR, NUM_ (2));

    return DIV_ (numer, denominator);
}

node_t* dPow (node_t* node)
{
    assert (node != NULL);

    if (node->data.op[0] != '^')
        return NULL;

    node_t* first_summand = MUL_ (MUL_ (LN_ (cL), c (node)), dR);

    node_t* second_summand = MUL_ (MUL_ (cR, POW_ (cL, SUB_ (cR, NUM_ (1)))), dL);

    return ADD_ (first_summand, second_summand);
}

node_t* dSqrt (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "sqrt") != 0)
        return NULL;

    return MUL_ (DIV_ (NUM_ (0.5), c (node)), dR);
}

node_t* dLn (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "ln") != 0)
        return NULL;

    return MUL_ (DIV_ (NUM_ (1), cR), dR);
}

node_t* dSin (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "sin") != 0)
        return NULL;

    return MUL_ (COS_ (cR), dR);
}

node_t* dCos (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "cos") != 0)
        return NULL;

    return SUB_ (NUM_ (0), MUL_ (SIN_ (cR), dR));
}

node_t* dTg (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "tg") != 0)
        return NULL;

    return MUL_ (DIV_ (NUM_ (1), POW_ (COS_ (cR), NUM_ (2))), dR);
}

node_t* dCtg (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "ctg") != 0)
        return NULL;

    node_t* new_node = MUL_ (DIV_ (NUM_ (1), POW_ (SIN_ (cR), NUM_ (2))), dR);

    return SUB_ (NUM_ (0), new_node);
}

node_t* dArcsin (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "arcsin") != 0)
        return NULL;

    node_t* external_der = DIV_ (NUM_ (1), SQRT_ (SUB_ (NUM_ (1), POW_ (cR, NUM_ (2)))));

    return MUL_ (external_der, dR);
}

node_t* dArccos (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "arccos") != 0)
        return NULL;

    node_t* external_der = DIV_ (NUM_ (1), SQRT_ (SUB_ (NUM_ (1), POW_ (cR, NUM_ (2)))));

    return SUB_ (NUM_ (0), MUL_ (external_der, dR));
}

node_t* dArctg (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "arccos") != 0)
        return NULL;

    node_t* external_der = DIV_ (NUM_ (1), ADD_ (NUM_ (1), POW_ (cR, NUM_ (2))));

    return MUL_ (external_der, dR);
}

node_t* dArcctg (node_t* node)
{
    assert (node != NULL);

    if (strcmp (node->data.func, "arccos") != 0)
        return NULL;

    node_t* external_der = DIV_ (NUM_ (1), ADD_ (NUM_ (1), POW_ (cR, NUM_ (2))));

    return SUB_ (NUM_ (0), MUL_ (external_der, dR));
}
