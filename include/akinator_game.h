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

void menu(Node_t* root, const char* FILENAME_DATA_BASE);
Node_t* build_tree(Node_t* root, char** string_buffer, size_t* line_number, size_t number_of_string);
