#pragma once

#define GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

#define R node->right
#define L node->left

enum child_node_t {
    LEFT =  -1,
    RIGHT = 1,
};

enum type_of_expr {
    NUM,
    VAR,
    OP,
    FUNC
};

enum code_of_func {
    SQRT,
    LN,
    SIN,
    COS,
    TG,
    CTG,
    ARCSIN,
    ARCCOS,
    ARCTG,
    ARCCTG,

    COUNT_OF_FUNC
};

enum code_of_op {
    ADD,
    SUB,
    MUL,
    DIV,
    POW,

    COUNT_OF_OP
};

struct func_t {
    char name[10];
    code_of_func code;
};

struct op_t {
    char name;
    code_of_op code;
};

union data_t {
    double num;
    char var[10];
    code_of_op op;
    code_of_func func;
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

const char list_of_var[][2] = {
    "x",
    "y",
    "z"
};

extern func_t list_of_func[];
extern op_t list_of_op[];

const int COUNT_OF_VAR = 3;

const int EXTRA_SIZE = 10;


void PrintNode (node_t* node);

node_t* Optimize (tree_t* tree);

tree_t* InitTree ();
node_t* InitNode ();
node_t* NewNode (type_of_expr expression, data_t value,
                  node_t* left_node, node_t* right_node);
size_t CountTreeSize (tree_t* tree);
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

node_t* DiffNode (node_t* node);
node_t* CopyNode (node_t* node);

tree_t* CreateTreeFromFile ();
char* ReadInput (FILE* input);
node_t* GetExpression (char* s);

#define OPTIMIZE_IF_NUM_OP_NUM(operation)                                           \
    if (node->left->expr == NUM && node->right->expr == NUM) {                      \
        node->expr = NUM;                                                           \
                                                                                    \
        node->data.num = DeleteNodeAndRetData (node, LEFT).num operation            \
                            DeleteNodeAndRetData (node, RIGHT).num;                 \
                                                                                    \
        *is_tree_changed = true;                                                    \
                                                                                    \
        return node;                                                                \
    }
