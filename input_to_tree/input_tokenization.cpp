#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../headers/differentiator.h"
#include "../headers/tokens.h"
#include "../headers/nametable.h"

bool TokenizeNum (const char* input_string, token_array_t* token_array,
                  size_t* input_index);
bool TokenizeVar (const char* input_string, token_array_t* token_array,
                  size_t* input_index, nametable_t* nametable);

void TokenizeInput (const char* input_string, token_array_t* token_array,  nametable_t* nametable)
{
    assert (input_string);
    assert (token_array);

    size_t input_index = 0;

    while (input_string[input_index] != '\0') {
        // printf (GREEN "PASSED\n" COLOR_RESET);
        // printf ("we see '%c' [%d]\n", input_string[input_index], input_string[input_index]);
        // printf ("\n");
        if (token_array->capacity - token_array->size == 1)
            ResizeTokenArray (token_array);

        if (input_string[input_index] == ' ' ||
            input_string[input_index] == '\t' ||
            input_string[input_index] == '\n') {
            input_index++;
            continue;
        }

        TOKENIZE_OP ("(", LEFT_BRACKET_TOKEN);
        TOKENIZE_OP (")", RIGHT_BRACKET_TOKEN);
        TOKENIZE_OP ("+", ADD_TOKEN);
        TOKENIZE_OP ("-", SUB_TOKEN);
        TOKENIZE_OP ("*", MUL_TOKEN);
        TOKENIZE_OP ("/", DIV_TOKEN);
        TOKENIZE_OP ("^", POW_TOKEN);

        TOKENIZE_FUNC ("sqrt",   4, SQRT,   SQRT_TOKEN);
        TOKENIZE_FUNC ("ln",     2, LN,     LN_TOKEN);
        TOKENIZE_FUNC ("sin",    3, SIN,    SIN_TOKEN);
        TOKENIZE_FUNC ("cos",    3, COS,    COS_TOKEN);
        TOKENIZE_FUNC ("tg",     2, TG,     TG_TOKEN);
        TOKENIZE_FUNC ("ctg",    3, CTG,    CTG_TOKEN);
        TOKENIZE_FUNC ("arcsin", 6, ARCSIN, ARCSIN_TOKEN);
        TOKENIZE_FUNC ("arccos", 6, ARCCOS, ARCCOS_TOKEN);
        TOKENIZE_FUNC ("arctg",  5, ARCTG,  ARCTG_TOKEN);
        TOKENIZE_FUNC ("arcctg", 6, ARCCTG, ARCCTG_TOKEN);

        if (TokenizeNum (input_string, token_array, &input_index))
            continue;

        if (TokenizeVar (input_string, token_array, &input_index, nametable))
            continue;
    }
}

bool TokenizeNum (const char* input_string, token_array_t* token_array,
                  size_t* input_index)
{
    assert (input_string);
    assert (token_array);
    assert (input_index);

    int val = 0;

    if (input_string[*input_index] < '0' || input_string[*input_index] > '9')
        return false;

    while ('0' <= input_string[*input_index] && input_string[*input_index] <= '9') {
        val = 10 * val + input_string[*input_index] - '0';
        ++*input_index;
    }

    token_array->data[token_array->size].code = NUM_TOKEN;
    token_array->data[token_array->size].type.num = val;

    ++token_array->size;

    return true;
}


bool TokenizeVar (const char* input_string, token_array_t* token_array,
                  size_t* input_index, nametable_t* nametable)
{
    assert (input_string);
    assert (token_array);
    assert (input_index);
    assert (nametable);

    if ((input_string[*input_index] < 'a' || input_string[*input_index] > 'z') && input_string[*input_index] != '_')
        return false;

    token_array->data[token_array->size].code = VAR_TOKEN;

    char var[INIT_VAR_SIZE] = {};
    int index = 0;

    do {
        var[index++] = input_string[*input_index];
        ++*input_index;
    } while (('a' <= input_string[*input_index] && input_string[*input_index] <= 'z')
            || input_string[*input_index] == '_'
            || ('0' <= input_string[*input_index] && input_string[*input_index] <= '9'));

    strcpy (token_array->data[token_array->size].type.var.name, var);
    ++token_array->size;

    PasteToNametable (nametable, var);

    return true;
}
