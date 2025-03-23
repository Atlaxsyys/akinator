#pragma once

#include "tree.h"

const int SIZE_ANSWER = 20;
const int SIZE_QUESTION = 20; 

Tree_errors akinator(Node_t* root);
Tree_errors add_new_node(Node_t* current);
Tree_errors game(Node_t* root);