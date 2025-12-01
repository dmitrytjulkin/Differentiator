#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "tree.h"

int* SkipBrackets (char* input_array, int* inp_arr_index, char* cmd, int* diff_brackets);
node_t* ReturnIfCmdNum (char* input_array, int* inp_arr_index, int* diff_brackets, data_t cmd);
node_t* ReturnIfCmdFunc (char* input_array, int* inp_arr_index, int* diff_brackets, data_t cmd);
node_t* ReturnIfCmdOp (char* input_array, int* inp_arr_index, int* diff_brackets, data_t cmd);
node_t* ReturnIfCmdVar (char* input_array, int* inp_arr_index, int* diff_brackets, data_t cmd);
double atof (char* s);

void RunGraphDump (node_t* root, const char* name_of_file,
                   const char* cmd_to_launch_graph_dump)
{
    assert (root != NULL);
    assert (name_of_file != NULL);
    assert (cmd_to_launch_graph_dump != NULL);

    ClearGraphDump (name_of_file);

    FILE* output_ptr = fopen (name_of_file, "a");

    assert (output_ptr != NULL);

    PrintTreeInGraphDump (root, output_ptr);

    fclose (output_ptr);

    system (cmd_to_launch_graph_dump);
}

node_t* CreateTreeFromFile ()
{
    FILE* input_ptr = fopen ("input.txt", "r");

    assert (input_ptr != NULL);

    char* input_array = ReadInput (input_ptr);

    assert (input_array != NULL);

    int inp_arr_index = 0;
    int diff_brackets = 0;

    node_t* root = AddNodeFromFile (input_array, &inp_arr_index, &diff_brackets);

    fclose (input_ptr);
    free (input_array);

    return root;
}

node_t* AddNodeFromFile (char* input_array, int* inp_arr_index, int* diff_brackets)
{
    assert (diff_brackets != NULL);
    assert (inp_arr_index != NULL);
    assert (input_array != NULL);

    data_t cmd = {.func = ""};
    int count_of_read_char = 0;
    node_t* node = InitNode ();

    sscanf (&input_array[*inp_arr_index], " %s %n", cmd.func, &count_of_read_char);

    *inp_arr_index += count_of_read_char;

    if (SkipBrackets (input_array, inp_arr_index, cmd.func, diff_brackets) == NULL)
        return NULL;

    if (strcmp (cmd.func, "nil") == 0)
        return NULL;

    if ((node = ReturnIfCmdNum (input_array, inp_arr_index, diff_brackets, cmd)) != NULL)
        return node;

    if ((node = ReturnIfCmdFunc (input_array, inp_arr_index, diff_brackets, cmd)) != NULL)
        return node;

    if ((node = ReturnIfCmdOp (input_array, inp_arr_index, diff_brackets, cmd)) != NULL)
        return node;

    if ((node = ReturnIfCmdVar (input_array, inp_arr_index, diff_brackets, cmd)) != NULL)
        return node;

    return NULL;
}

int* SkipBrackets (char* input_array, int* inp_arr_index, char* cmd, int* diff_brackets)
{
    assert (input_array != NULL);
    assert (inp_arr_index != NULL);
    assert (cmd != NULL);
    assert (diff_brackets != NULL);

    while (strcmp (cmd, "(") == 0 || strcmp (cmd, ")") == 0
           || *diff_brackets == 0) {
        if (strcmp (cmd, "(") == 0)
            ++*diff_brackets;

        if (strcmp (cmd, ")") == 0)
            --*diff_brackets;

        if (*diff_brackets == 0)
            return NULL;

        int count_of_read_char = 0;

        sscanf (&input_array[*inp_arr_index], " %s %n", cmd, &count_of_read_char);

        *inp_arr_index += count_of_read_char;
    }

    return diff_brackets; // ~
}

node_t* ReturnIfCmdNum (char* input_array, int* inp_arr_index, int* diff_brackets, data_t cmd)
{
    if ('0' > cmd.func[0] || cmd.func[0] > '9') {
        return NULL;
    }

    cmd.num = atof (cmd.func);

    return NewNode (NUM, cmd,
                    AddNodeFromFile (input_array, inp_arr_index, diff_brackets),
                    AddNodeFromFile (input_array, inp_arr_index, diff_brackets));
}

node_t* ReturnIfCmdFunc (char* input_array, int* inp_arr_index, int* diff_brackets, data_t cmd)
{
    for (int num_of_func = 0; num_of_func < COUNT_OF_FUNC; ++num_of_func)
        if (strcmp (cmd.func, list_of_func[num_of_func]) == 0)
            return NewNode (FUNC, cmd,
                            AddNodeFromFile (input_array, inp_arr_index, diff_brackets),
                            AddNodeFromFile (input_array, inp_arr_index, diff_brackets));

    return NULL;
}

node_t* ReturnIfCmdOp (char* input_array, int* inp_arr_index, int* diff_brackets, data_t cmd)
{
    for (int num_of_op = 0; num_of_op < COUNT_OF_OP; ++num_of_op)
        if (strcmp (cmd.op, list_of_op[num_of_op]) == 0)
            return NewNode (OP, cmd,
                            AddNodeFromFile (input_array, inp_arr_index, diff_brackets),
                            AddNodeFromFile (input_array, inp_arr_index, diff_brackets));

    return NULL;
}

node_t* ReturnIfCmdVar (char* input_array, int* inp_arr_index, int* diff_brackets, data_t cmd)
{
    for (int num_of_var = 0; num_of_var < COUNT_OF_VAR; ++num_of_var)
        if (strcmp (cmd.var, list_of_var[num_of_var]) == 0)
            return NewNode (VAR, cmd,
                            AddNodeFromFile (input_array, inp_arr_index, diff_brackets),
                            AddNodeFromFile (input_array, inp_arr_index, diff_brackets));

    return NULL;
}

double atof (char* s)
{
    double int_part = 0;
    double rest_part = 0;

    double counter = 0;
    double count_of_char = 0;
    int is_there_a_dot = false;

    for (int i = 0; s[i] != '\0'; ++i) {
        if (s[i] == '.') {
            int_part = counter;
            counter = 0;
            count_of_char = 0;

            is_there_a_dot = true;

            continue;
        }

        ++count_of_char;
        counter = 10 * counter + s[i] - '0';
    }

    if (is_there_a_dot)
        rest_part = counter / pow (10, count_of_char);

    else
        int_part = counter;

    return int_part + rest_part;
}
