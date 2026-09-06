#pragma once

#include "../work_with_nametable/nametable.h"

enum type_of_expr {
    NUM,
    VAR,
    OP,
    FUNC,
    DIFF_VAR
};

enum code_of_func {
    SQRT,
    LN,
    SIN,
    COS,
    TG,
    CTG,
    ARCSIN,
    ARCCOS,
    ARCTG,
    ARCCTG,

    COUNT_OF_FUNC
};

enum code_of_op {
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    EQUALITY,

    COUNT_OF_OP
};

struct func_t {
    char name[10];
    code_of_func code;
};

struct op_t {
    char name;
    code_of_op code;
};

union data_t {
    double num;
    var_t var;
    code_of_op op;
    code_of_func func;
};

const char list_of_var[][2] = {
    "x",
    "y",
    "z"
};

extern func_t list_of_func[];
extern op_t list_of_op[];

const int COUNT_OF_VAR = 3;

