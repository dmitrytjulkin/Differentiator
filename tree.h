#pragma once

#define GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

enum child_node_t {
    LEFT =  -1,
    RIGHT = 1,
};

enum type_of_expr {
    NO_EXPR,
    NUM,
    VAR,
    OP,
    FUNC
};

enum type_of_func {
    CONST_func,
    VAR_func,
    SUM_func,
    MUL_func,
    DIV_func,
    EXP_func
};

union data_t {
    double num;
    char var[10];
    char op[2];
    char func[10];
};

struct node_t {
    node_t* parent;
    type_of_expr expr;
    data_t data;
    node_t* left;
    node_t* right;
};

const char list_of_func[][10] = {
    "sqrt",
    "ln",
    "sin",
    "cos",
    "tg",
    "ctg",
    "arcsin",
    "arccos",
    "arctg",
    "arcctg"
};

const char list_of_op[][2] = {
    "+",
    "-",
    "*",
    "/",
    "%",
    "^"
};

const char list_of_var[][2] = {
    "x",
    "y",
    "z"
};

const int COUNT_OF_FUNC = 10;
const int COUNT_OF_OP = 6;
const int COUNT_OF_VAR = 3;

const int EXTRA_SIZE = 10;

char* ReadInput (FILE* input);
node_t* CreateTreeFromFile ();
bool IsZero (double a);

node_t* Optimize (node_t* node);

node_t* InitNode ();
node_t* NewNode (type_of_expr expression, data_t value,
                  node_t* left_node, node_t* right_node);
void FreeTree (node_t* node);
data_t DeleteNodeAndRetData (node_t* node, child_node_t dir_of_child);

void RunGraphDump (node_t* root, const char* name_of_file,
                   const char* cmd_to_launch_graph_dump);
void PrintTreeInGraphDump (node_t* root, FILE* output_ptr);
void PrintNodeInGraphDump (FILE* output_ptr, node_t* node, node_t* child,
                           const char* link_for_arrow);
void ClearGraphDump (const char* name_of_file);

node_t* d (node_t* node);
node_t* c (node_t* node);

void SyntaxError (const char* funcname, int line);
node_t* GetG (char** s);
node_t* GetE (char** s);
node_t* GetT (char** s);
node_t* GetP (char** s);
node_t* GetN (char** s);
node_t* GetV (char** s);
node_t* GetF (char** s, node_t* var);
node_t* GetPow (char** s);


#define OPTIMIZE_NUM_OP_NUM(operation)                                              \
        if (node->data.op[0] == #operation[0]) {                                    \
            node->expr = NUM;                                                       \
                                                                                    \
            node->data.num = DeleteNodeAndRetData (node, LEFT).num  operation       \
                             DeleteNodeAndRetData (node, RIGHT).num;                \
                                                                                    \
            return node;                                                            \
        }
