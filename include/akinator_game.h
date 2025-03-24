#pragma once

#include "tree.h"

const int SIZE_ANSWER = 20;
const int SIZE_QUESTION = 20; 

enum akinator_menu
{
    PLAY              = 1,
    SHOW_DATA_BASE    = 2,
    EXIT_WITH_SAVING    = 3,
    EXIT_WITHOUT_SAVING = 4
};

Tree_errors akinator(Node_t* root);
Tree_errors add_new_node(Node_t* current);
Tree_errors game(Node_t* root);
void menu(Node_t* root, const char* argv[]);
void show_data_base();