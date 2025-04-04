#pragma once

#include "tree.h"
#include "errors.h"

const int SIZE_ANSWER = 20;
const int SIZE_QUESTION = 20; 

enum akinator_menu
{
    PLAY                = 1,
    SHOW_DATA_BASE      = 2,
    DEFINITION          = 3,
    COMPARE_NODES       = 4,
    EXIT_WITH_SAVING    = 5,
    EXIT_WITHOUT_SAVING = 6
};

Tree_errors akinator(Node_t* root);
Tree_errors add_new_node(Node_t* current);
void menu(Node_t* root, const char* FILENAME_DATA_BASE);
Tree_errors build_path(Node_t* root, Node_t* node, Path* pth);
Tree_errors compare_nodes(Node_t* root);
int get_common_part(Path first_path, Path second_path);
Tree_errors check_node_exists(Node_t* root, Node_t** node, const char* promts, char elem[]);
Tree_errors output_unique_features(Node_t* root, int common_part, char first_elem[], Path path);
Tree_errors output_common_features(int common_part, Path path);
Tree_errors exit_with_saving(Node_t* root,const char* FILENAME_DATA_BASE);
