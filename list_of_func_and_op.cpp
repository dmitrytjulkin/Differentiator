#include <stdio.h>

#include "tree.h"

func_t list_of_func[] = {
    {"sqrt",   SQRT},
    {"ln",     LN},
    {"sin",    SIN},
    {"cos",    COS},
    {"tg",     TG},
    {"ctg",    CTG},
    {"arcsin", ARCSIN},
    {"arccos", ARCCOS},
    {"arctg",  ARCTG},
    {"arcctg", ARCCTG}
};

op_t list_of_op[] = {
    {'+', ADD},
    {'-', SUB},
    {'*', MUL},
    {'/', DIV},
    {'^', POW},
};
