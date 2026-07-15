#pragma once

#include "nametable.h"
#include "func_and_op.h"

enum token_codes {
    FINISH_TOKEN,

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

    NUM_TOKEN,
    VAR_TOKEN,

    COUNT_OF_TOKENS
};

union token_data_t {
    int num;
    var_t var;
    code_of_func func;
};

struct token_t {
    token_codes code;
    token_data_t type;
};

struct token_array_t {
    token_t* data;
    size_t size;
    size_t capacity;
};

void InitTokenArray   (token_array_t* token_array);
void ResizeTokenArray  (token_array_t* token_array);
void DestroyTokenArray (token_array_t* token_array);

void TokenizeInput (const char* input_string, token_array_t* token_arr,  nametable_t* nametable);

#define TOKENIZE_OP(operation, op_code)                                 \
    if (input_string[input_index] == *operation){                       \
        token_array->data[token_array->size++].code = op_code;          \
        ++input_index;                                                  \
        continue;                                                       \
    }

#define TOKENIZE_FUNC(func_name, func_size, general_code, token_code)       \
    if (strncmp (input_string + input_index, func_name, func_size) == 0) {  \
        token_array->data[token_array->size].code = token_code;             \
        token_array->data[token_array->size++].type.func = general_code;    \
        input_index += func_size;                                           \
        continue;                                                           \
    }
