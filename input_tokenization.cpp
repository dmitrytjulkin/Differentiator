#include <stdio.h>
#include <assert.h>

#include "headers/differentiator.h"
#include "headers/tokens.h"
#include "headers/nametable.h"

void TokenizeInput (char* input_string, int* token_arr,  nametable_t* nametable)
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

        TokenizeVar ();
    }
}

bool TokenizeVar (const char** input_string, token_t* token_arr,
                  size_t* step, nametable_t* nametable)
{
    assert (input_string != NULL);
    assert (token_arr != NULL);
    assert (step != NULL);

    if ((**input_string < 'a' || **input_string > 'z')
        && **input_string != '_')
        return false;

    token_arr[*step].code = VAR_TOKEN;

    char var[INIT_VAR_SIZE] = { 0 };
    int index = 0;

    do {
        var[index++] = **input_string;
        ++*input_string;
    } while (('a' <= **input_string && **input_string <= 'z')
            || **input_string == '_'
            || ('0' <= **input_string && **input_string <= '9'));

    strcpy (token_arr[(*step)++], var);

    PasteToNametable (nametable, var);

    return true;
}
