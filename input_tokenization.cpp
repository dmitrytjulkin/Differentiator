#include <stdio.h>
#include <assert.h>

#include "headers/differentiator.h"

#define TOKENIZE_OP(operation, op_code)               \
    if (input_string[input_index] == operation) {    \
        token_arr[step++] = op_code;                  \
        input_index++                                \
        continue;                                     \
    }


int* TokenizeInput (char* input_string, int* token_arr)
{
    assert (input_string);
    // assert (input_index);
    assert (token_arr);

    int step = 0;
    int input_index = 0;

    while (input_string[input_index] != '\0') {
        if (input_string[input_index++] == ' ') continue;

        TOKENIZE_OP ("(", LEFT_BRACKET, 1);
        TOKENIZE_OP (")", RIGHT_BRACKET, 1);
        TOKENIZE_OP ("+", ADD, 1);
        TOKENIZE_OP ("-", SUB, 1);
        TOKENIZE_OP ("*", MUL, 1);
        TOKENIZE_OP ("/", DIV, 1);
        TOKENIZE_OP ("^", POW, 1);

        TOKENIZE_IT ("")

    }
}
