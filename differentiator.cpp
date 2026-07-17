#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "headers/differentiator.h"


#define diff(node) DiffNode(node, arg)
#define copy(node) CopyNode(node)

#define NUM_(number) NewNode (NUM, {.num = number}, NULL, NULL)

#define ADD_(left, right) NewNode (OP, {.op = ADD}, left, right)
#define SUB_(left, right) NewNode (OP, {.op = SUB}, left, right)
#define MUL_(left, right) NewNode (OP, {.op = MUL}, left, right)
#define DIV_(left, right) NewNode (OP, {.op = DIV}, left, right)
#define POW_(left, right) NewNode (OP, {.op = POW}, left, right)

#define SQRT_(right) NewNode (FUNC,  {.func = SQRT}, NULL, right)
#define LN_(right)   NewNode (FUNC,  {.func = LN}, NULL, right)
#define SIN_(right)  NewNode (FUNC,  {.func = SIN}, NULL, right)
#define COS_(right)  NewNode (FUNC,  {.func = COS}, NULL, right)

// static
node_t* dOp     (node_t* node, const char* arg);
node_t* dFunc   (node_t* node, const char* arg);

node_t* dDiv    (node_t* node, const char* arg);
node_t* dPow    (node_t* node, const char* arg);

node_t* dSqrt   (node_t* node, const char* arg);
node_t* dLn     (node_t* node, const char* arg);
node_t* dSin    (node_t* node, const char* arg);
node_t* dCos    (node_t* node, const char* arg);
node_t* dTg     (node_t* node, const char* arg);
node_t* dCtg    (node_t* node, const char* arg);
node_t* dArcsin (node_t* node, const char* arg);
node_t* dArccos (node_t* node, const char* arg);
node_t* dArctg  (node_t* node, const char* arg);
node_t* dArcctg (node_t* node, const char* arg);


node_t* DiffNode (node_t* node, const char* arg)
{
    if (node == NULL)
        return NULL;

    switch (node->expr) {
        case NUM:
            return NUM_ (0);

        case OP:
            return dOp (node, arg);

        case FUNC:
            return dFunc (node, arg);

        case VAR:
            if (strcmp (arg, node->data.var) == 0)
                return NUM_ (1);

            else
                return NUM_ (0);

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

    copy_node->left = copy (L);
    copy_node->right = copy (R);

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

node_t* dOp (node_t* node, const char* arg)
{
    assert (node);

    switch (node->data.op) {
        case ADD:
            return ADD_ (diff (L), diff (R));

        case SUB:
            return SUB_ (diff (L), diff (R));

        case MUL:
            return ADD_ (MUL_ (diff (L), copy (R)), MUL_ (copy (L), diff (R)));

        case DIV:
            return dDiv (node, arg);

        case POW:
            return dPow (node, arg);

        case COUNT_OF_OP: default:
            printf ("This type of op doesn't exist, "
                    "its code: %d\n", node->data.op);

            assert (0);
    }

}

node_t* dFunc (node_t* node, const char* arg)
{
    assert (node);

    switch (node->data.func) {
        case SQRT:
            return dSqrt (node, arg);

        case LN:
            return dLn (node, arg);

        case SIN:
            return dSin (node, arg);

        case COS:
            return dCos (node, arg);

        case TG:
            return dTg (node, arg);

        case CTG:
            return dCtg (node, arg);

        case ARCSIN:
            return dArcsin (node, arg);

        case ARCCOS:
            return dArccos (node, arg);

        case ARCTG:
            return dArctg (node, arg);

        case ARCCTG:
            return dArcctg (node, arg);

        case COUNT_OF_FUNC: default:
            printf ("This type of func doesn't exist, "
                    "its code: %d\n", node->data.func);

            assert (0);
    }
}

node_t* dDiv (node_t* node, const char* arg)
{
    assert (node);

    if (node->data.op != DIV)
        return NULL;

    node_t* numer = SUB_ (MUL_ (diff (L), copy (R)), MUL_ (copy (L), diff (R)));
    node_t* denominator = POW_ (copy (R), NUM_ (2));

    return DIV_ (numer, denominator);
}

node_t* dPow (node_t* node, const char* arg)
{
    assert (node);

    if (node->data.op != POW)
        return NULL;

    node_t* first_summand = MUL_ (MUL_ (LN_ (copy (L)), copy (node)), diff (R));

    node_t* second_summand = MUL_ (MUL_ (copy (R), POW_ (copy (L),
                             SUB_ (copy (R), NUM_ (1)))), diff (L));

    return ADD_ (first_summand, second_summand);
}

node_t* dSqrt (node_t* node, const char* arg)
{
    assert (node);

    return MUL_ (DIV_ (NUM_ (0.5), copy (node)), diff (R));
}

node_t* dLn (node_t* node, const char* arg)
{
    assert (node);

    return MUL_ (DIV_ (NUM_ (1), copy (R)), diff (R));
}

node_t* dSin (node_t* node, const char* arg)
{
    assert (node);

    return MUL_ (COS_ (copy (R)), diff (R));
}

node_t* dCos (node_t* node, const char* arg)
{
    assert (node);

    return SUB_ (NUM_ (0), MUL_ (SIN_ (copy (R)), diff (R)));
}

node_t* dTg (node_t* node, const char* arg)
{
    assert (node);

    return MUL_ (DIV_ (NUM_ (1), POW_ (COS_ (copy (R)), NUM_ (2))), diff (R));
}

node_t* dCtg (node_t* node, const char* arg)
{
    assert (node);

    node_t* external_der = DIV_ (NUM_ (1), POW_ (SIN_ (copy (R)), NUM_ (2)));

    return SUB_ (NUM_ (0),  MUL_ (external_der, diff (R)));
}

node_t* dArcsin (node_t* node, const char* arg)
{
    assert (node);

    node_t* external_der = DIV_ (NUM_ (1), SQRT_ (SUB_ (NUM_ (1), POW_ (copy (R), NUM_ (2)))));

    return MUL_ (external_der, diff (R));
}

node_t* dArccos (node_t* node, const char* arg)
{
    assert (node);

    node_t* external_der = DIV_ (NUM_ (1), SQRT_ (SUB_ (NUM_ (1), POW_ (copy (R), NUM_ (2)))));

    return SUB_ (NUM_ (0), MUL_ (external_der, diff (R)));
}

node_t* dArctg (node_t* node, const char* arg)
{
    assert (node);

    node_t* external_der = DIV_ (NUM_ (1), ADD_ (NUM_ (1), POW_ (copy (R), NUM_ (2))));

    return MUL_ (external_der, diff (R));
}

node_t* dArcctg (node_t* node, const char* arg)
{
    assert (node);

    node_t* external_der = DIV_ (NUM_ (1), ADD_ (NUM_ (1), POW_ (copy (R), NUM_ (2))));

    return SUB_ (NUM_ (0), MUL_ (external_der, diff (R)));
}
