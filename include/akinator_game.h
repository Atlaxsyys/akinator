#pragma once

#include "tree.h"

const int SIZE_ANSWER = 20;
const int SIZE_QUESTION = 20; 

enum akinator_menu
{
    PLAY                = 1,
    SHOW_DATA_BASE      = 2,
    COMPARE_NODES       = 3,
    EXIT_WITH_SAVING    = 4,
    EXIT_WITHOUT_SAVING = 5
};

Tree_errors akinator(Node_t* root);
Tree_errors add_new_node(Node_t* current);
Tree_errors game(Node_t* root);
void menu(Node_t* root, const char* FILENAME_DATA_BASE);
Tree_errors build_path(Node_t* root, Node_t* node, Path* pth);
Tree_errors compare_nodes(Node_t* root);
Tree_errors verify_nodes(Node_t* root, Node_t** first_node, Node_t** second_node, char first_elem[], char second_elem[]);
Tree_errors output_unique_features(Node_t* root, int common_part, char first_elem[], Path path);
Tree_errors output_common_features(int common_part, Path path);
Tree_errors exit_with_saving(Node_t* root,const char* FILENAME_DATA_BASE);
