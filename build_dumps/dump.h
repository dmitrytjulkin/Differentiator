#pragma once

#include "../parse_tree/tree.h"
#include "../differentiator/differentiator.h"

void RunGraphDump (tree_t* tree, const char* name_of_file,
                   const char* cmd_to_launch_graph_dump);
void PrintTreeInGraphDump (node_t* root, FILE* output_ptr);
void PrintNodeInGraphDump (FILE* output_ptr, node_t* node, node_t* child,
                           const char* link_for_arrow);
void ClearDump (const char* name_of_file);

void RunTexDump (const char* name_of_file, tree_t* tree);
void AddTexLine (const char* name_of_file, node_t* node, const char* phrase);
void FinishTex  (const char* name_of_file);

