#pragma once

#include "nametable.h"

enum token_codes {
    ADD_TOKEN,
    SUB_TOKEN,
    MUL_TOKEN,
    DIV_TOKEN,
    POW_TOKEN,
    LEFT_BRACKET_TOKEN,
    RIGHT_BRACKET_TOKEN,

    SQRT_TOKEN,
    LN_TOKEN,
    SIN_TOKEN,
    COS_TOKEN,
    TG_TOKEN,
    CTG_TOKEN,
    ARCSIN_TOKEN,
    ARCCOS_TOKEN,
    ARCTG_TOKEN,
    ARCCTG_TOKEN,

    VAR_TOKEN,

    COUNT_OF_TOKENS
};

struct token_t {
    token_codes code;
    var_t data;
};

void TokenizeInput (char* input_string, token_t* token_arr);

#define TOKENIZE_OP(operation, op_code, op_size)                        \
    if (strcmp(input_string[input_index], operation, op_size) == 0){    \
        token_arr[step++].code = op_code;                                    \
        input_index += op_size;                                         \
        continue;                                                       \
    }
