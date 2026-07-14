#include <stdio.h>
#include <assert.h>

#include "headers/differentiator.h"
#include "headers/tokens.h"
#include "headers/nametable.h"

bool TokenizeNum (const char** input_string, token_array_t* token_array,
                  size_t* token_array_index, size_t* input_index);
bool TokenizeVar (const char** input_string, token_array_t* token_array,
                  size_t* token_array_index, size_t* input_index, nametable_t* nametable);
void PasteToNametable (nametable_t* nametable, char* var);

void TokenizeInput (char* input_string, token_array_t* token_array,  nametable_t* nametable)
{
    assert (input_string);
    assert (token_array);

    size_t token_array_index = 0;
    size_t input_index = 0;

    while (input_string[input_index] != '\0') {
        if (token_array->capacity - token_array->size == 1)
            ResizeTokenArray (token_array);

        if (input_string[input_index] == ' ' ||
            input_string[input_index] == '\t' ||
            input_string[input_index] == '\n') {
            input_index++;
            continue;
        }

        TOKENIZE_OP ("(", LEFT_BRACKET_TOKEN, 1);
        TOKENIZE_OP (")", RIGHT_BRACKET_TOKEN, 1);
        TOKENIZE_OP ("+", ADD_TOKEN, 1);
        TOKENIZE_OP ("-", SUB_TOKEN, 1);
        TOKENIZE_OP ("*", MUL_TOKEN, 1);
        TOKENIZE_OP ("/", DIV_TOKEN, 1);
        TOKENIZE_OP ("^", POW_TOKEN, 1);

        TOKENIZE_OP ("sqrt",   SQRT_TOKEN,   4);
        TOKENIZE_OP ("ln",     LN_TOKEN,     2);
        TOKENIZE_OP ("sin",    SIN_TOKEN,    3);
        TOKENIZE_OP ("cos",    COS_TOKEN,    3);
        TOKENIZE_OP ("tg",     TG_TOKEN,     2);
        TOKENIZE_OP ("ctg",    CTG_TOKEN,    3);
        TOKENIZE_OP ("arcsin", ARCSIN_TOKEN, 6);
        TOKENIZE_OP ("arccos", ARCCOS_TOKEN, 6);
        TOKENIZE_OP ("arctg",  ARCTG_TOKEN,  5);
        TOKENIZE_OP ("arcctg", ARCCTG_TOKEN, 6);

        if (TokenizeNum (&input_string, token_array, &token_array_index, &input_index))
            continue;

        if (TokenizeVar (&input_string, token_array, &token_array_index, &input_index, nametable))
            continue;
    }
}

bool TokenizeNum (const char** input_string, token_array_t* token_array,
                  size_t* token_array_index, size_t* input_index)
{
    assert (input_string);
    assert (token_array);
    assert (token_array_index);
    assert (input_index);

    int val = 0;

    if (*input_string[*input_index] < '0' || *input_string[*input_index] > '9')
        return false;

    while ('0' <= *input_string[*input_index] && *input_string[*input_index] <= '9') {
        val += 10 * val + *input_string[*input_index] - '0';
        ++*input_index;
    }

    token_array->data[*token_array_index].code = NUM_TOKEN;
    token_array->data[*token_array_index].type.num = val;

    ++*token_array_index;

    return true;
}


bool TokenizeVar (const char** input_string, token_array_t* token_array,
                  size_t* token_array_index, size_t* input_index, nametable_t* nametable)
{
    assert (input_string);
    assert (token_array);
    assert (token_array_index);
    assert (input_index);
    assert (nametable);

    if ((**input_string < 'a' || **input_string > 'z') && **input_string != '_')
        return false;

    token_array.data[*token_array_index].code = VAR_TOKEN;

    char var[INIT_VAR_SIZE] = {};
    int index = 0;

    do {
        var[index++] = **input_string;
        ++*input_string;
    } while (('a' <= **input_string && **input_string <= 'z')
            || **input_string == '_'
            || ('0' <= **input_string && **input_string <= '9'));

    strcpy (token_array->data[*token_array_index].type.var.name, var);
    ++*token_array_index;

    PasteToNametable (nametable, var);

    return true;
}

void PasteToNametable (nametable_t* nametable, char* var)
{
    assert (nametable);
    assert (var);

    size_t index = 0;

    while (index < nametable->size) {
        if (strcmp (nametable->data[index].name, var) == 0)
            return;

        ++index;
    }

    strcpy (nametable->data[nametable->size].name, var);
    ++nametable->size;

    if (nametable->capacity - nametable->size <= 2)
        ResizeNametable (nametable);
}
