#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"


#define d DiffNode
#define c CopyNode

#define NUM_(number) NewNode (NUM, {.num = number}, NULL, NULL)

#define ADD_(left, right) NewNode (OP, {.op = ADD}, left, right)
#define SUB_(left, right) NewNode (OP, {.op = SUB}, left, right)
#define MUL_(left, right) NewNode (OP, {.op = MUL}, left, right)
#define DIV_(left, right) NewNode (OP, {.op = DIV}, left, right)
#define POW_(left, right) NewNode (OP, {.op = POW}, left, right)

#define SQRT_(right) NewNode (FUNC, {.func = SQRT}, NULL, right)
#define LN_(right)   NewNode (FUNC,   {.func = LN}, NULL, right)
#define SIN_(right)  NewNode (FUNC,  {.func = SIN}, NULL, right)
#define COS_(right)  NewNode (FUNC,  {.func = COS}, NULL, right)


node_t* dOp     (node_t* node);
node_t* dFunc   (node_t* node);

node_t* dDiv    (node_t* node);
node_t* dPow    (node_t* node);

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


node_t* DiffNode (node_t* node)
{
    if (node == NULL)
        return NULL;

    switch (node->expr) {
        case NUM:
            return NUM_ (0);

        case VAR:
            return NUM_ (1);

        case OP:
            return dOp (node);

        case FUNC:
            return dFunc (node);

        default:
            assert (0);
    }

    return NULL;
}

node_t* CopyNode (node_t* node)
{
    if (node == NULL)
        return NULL;

    node_t* copy_node = InitNode ();

    copy_node->left = c (L);
    copy_node->right = c (R);

    copy_node->expr = node->expr;

    if (node->expr == NUM)
        copy_node->data.num = node->data.num;

    else if (node->expr == VAR)
        strcpy (copy_node->data.var, node->data.var);

    else if (node->expr == OP)
        copy_node->data.op = node->data.op;

    else
        copy_node->data.func = node->data.func;

    return copy_node;
}

node_t* dOp (node_t* node)
{
    assert (node != NULL);

    switch (node->data.op) {
        case ADD:
            return ADD_ (d (L), d (R));

        case SUB:
            return SUB_ (d (L), d (R));

        case MUL:
            return ADD_ (MUL_ (d (L), c (R)), MUL_ (c (L), d (R)));

        case DIV:
            return dDiv (node);

        case POW:
            return dPow (node);

        case COUNT_OF_OP: default:
            printf ("This type of op doesn't exist, "
                    "its code: %d\n", node->data.op);

            assert (0);
    }

}

node_t* dFunc (node_t* node)
{
    assert (node != NULL);

    switch (node->data.func) {
        case SQRT:
            return dSqrt (node);

        case LN:
            return dLn (node);

        case SIN:
            return dSin (node);

        case COS:
            return dCos (node);

        case TG:
            return dTg (node);

        case CTG:
            return dCtg (node);

        case ARCSIN:
            return dArcsin (node);

        case ARCCOS:
            return dArccos (node);

        case ARCTG:
            return dArctg (node);

        case ARCCTG:
            return dArcctg (node);

        case COUNT_OF_FUNC: default:
            printf ("This type of func doesn't exist, "
                    "its code: %d\n", node->data.func);

            assert (0);
    }
}

node_t* dDiv (node_t* node)
{
    assert (node != NULL);

    if (node->data.op != DIV)
        return NULL;

    node_t* numer = SUB_ (MUL_ (d (L), c (R)), MUL_ (c (L), d (R)));
    node_t* denominator = POW_ (c (R), NUM_ (2));

    return DIV_ (numer, denominator);
}

node_t* dPow (node_t* node)
{
    assert (node != NULL);

    if (node->data.op != POW)
        return NULL;

    node_t* first_summand = MUL_ (MUL_ (LN_ (c (L)), c (node)), d (R));

    node_t* second_summand = MUL_ (MUL_ (c (R), POW_ (c (L), SUB_ (c (R), NUM_ (1)))), d (L));

    return ADD_ (first_summand, second_summand);
}

node_t* dSqrt (node_t* node)
{
    assert (node != NULL);

    return MUL_ (DIV_ (NUM_ (0.5), c (node)), d (R));
}

node_t* dLn (node_t* node)
{
    assert (node != NULL);

    return MUL_ (DIV_ (NUM_ (1), c (R)), d (R));
}

node_t* dSin (node_t* node)
{
    assert (node != NULL);

    return MUL_ (COS_ (c (R)), d (R));
}

node_t* dCos (node_t* node)
{
    assert (node != NULL);

    return SUB_ (NUM_ (0), MUL_ (SIN_ (c (R)), d (R)));
}

node_t* dTg (node_t* node)
{
    assert (node != NULL);

    return MUL_ (DIV_ (NUM_ (1), POW_ (COS_ (c (R)), NUM_ (2))), d (R));
}

node_t* dCtg (node_t* node)
{
    assert (node != NULL);

    node_t* external_der = DIV_ (NUM_ (1), POW_ (SIN_ (c (R)), NUM_ (2)));

    return SUB_ (NUM_ (0),  MUL_ (external_der, d (R)));
}

node_t* dArcsin (node_t* node)
{
    assert (node != NULL);

    node_t* external_der = DIV_ (NUM_ (1), SQRT_ (SUB_ (NUM_ (1), POW_ (c (R), NUM_ (2)))));

    return MUL_ (external_der, d (R));
}

node_t* dArccos (node_t* node)
{
    assert (node != NULL);

    node_t* external_der = DIV_ (NUM_ (1), SQRT_ (SUB_ (NUM_ (1), POW_ (c (R), NUM_ (2)))));

    return SUB_ (NUM_ (0), MUL_ (external_der, d (R)));
}

node_t* dArctg (node_t* node)
{
    assert (node != NULL);

    node_t* external_der = DIV_ (NUM_ (1), ADD_ (NUM_ (1), POW_ (c (R), NUM_ (2))));

    return MUL_ (external_der, d (R));
}

node_t* dArcctg (node_t* node)
{
    assert (node != NULL);

    node_t* external_der = DIV_ (NUM_ (1), ADD_ (NUM_ (1), POW_ (c (R), NUM_ (2))));

    return SUB_ (NUM_ (0), MUL_ (external_der, d (R)));
}
