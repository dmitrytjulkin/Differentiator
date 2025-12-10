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

struct tree_t {
    node_t* root;
    size_t size;
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

tree_t* CreateTreeFromFile ();
bool IsZero (double a);

node_t* Optimize (tree_t* tree);

tree_t* InitTree ();
node_t* InitNode ();
node_t* NewNode (type_of_expr expression, data_t value,
                  node_t* left_node, node_t* right_node);
void FreeTree (tree_t* tree);
void FreeNode (node_t* node);
data_t DeleteNodeAndRetData (node_t* node, child_node_t dir_of_child);

void RunGraphDump (tree_t* tree, const char* name_of_file,
                   const char* cmd_to_launch_graph_dump);
void PrintTreeInGraphDump (node_t* root, FILE* output_ptr);
void PrintNodeInGraphDump (FILE* output_ptr, node_t* node, node_t* child,
                           const char* link_for_arrow);
void ClearDump (const char* name_of_file);

void RunTexDump (tree_t* tree, tree_t* der_tree);

node_t* d (node_t* node);
node_t* c (node_t* node);

char* ReadInput (FILE* input);
node_t* GetG (char* s);

#define OPTIMIZE_NUM_OP_NUM(operation)                                              \
        if (node->data.op[0] == #operation[0]) {                                    \
            node->expr = NUM;                                                       \
                                                                                    \
            node->data.num = DeleteNodeAndRetData (node, LEFT).num  operation       \
                             DeleteNodeAndRetData (node, RIGHT).num;                \
                                                                                    \
            return node;                                                            \
        }

#define APPLY_OPTIMIZE(function)                                                    \
    if ((new_node = function (node, is_tree_changed)) != NULL)                      \
        return new_node;

#define SKIP_OR_DO_AND_RETURN(function)                                             \
    if ((new_node = function) != NULL)                                              \
        return new_node;
