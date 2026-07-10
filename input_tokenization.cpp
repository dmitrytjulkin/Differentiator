#include <stdio.h>
#include <assert.h>

#include "headers/differentiator.h"
#include "headers/tokens.h"
#include "headers/nametable.h"

void TokenizeInput (char* input_string, int* token_arr)
{
    assert (input_string);
    assert (token_arr);

    int step = 0;
    int input_index = 0;

    while (input_string[input_index] != '\0') {
        if (input_string[input_index] == ' ' ||
            input_string[input_index] == '\t' ||
            input_string[input_index] == '\n') {
            input_index++;
            continue;
        }

        TOKENIZE_OP ("(", LEFT_BRACKET, 1);
        TOKENIZE_OP (")", RIGHT_BRACKET, 1);
        TOKENIZE_OP ("+", ADD, 1);
        TOKENIZE_OP ("-", SUB, 1);
        TOKENIZE_OP ("*", MUL, 1);
        TOKENIZE_OP ("/", DIV, 1);
        TOKENIZE_OP ("^", POW, 1);

        TOKENIZE_IT ("sqrt",   SQRT_TOKEN,   4);
        TOKENIZE_IT ("ln",     LN_TOKEN,     2);
        TOKENIZE_IT ("sin",    SIN_TOKEN,    3);
        TOKENIZE_IT ("cos",    COS_TOKEN,    3);
        TOKENIZE_IT ("tg",     TG_TOKEN,     2);
        TOKENIZE_IT ("ctg",    CTG_TOKEN,    3);
        TOKENIZE_IT ("arcsin", ARCSIN_TOKEN, 6);
        TOKENIZE_IT ("arccos", ARCCOS_TOKEN, 6);
        TOKENIZE_IT ("arctg",  ARCTG_TOKEN,  5);
        TOKENIZE_IT ("arcctg", ARCCTG_TOKEN, 6);


    }
}
