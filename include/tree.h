#pragma once

#include "errors.h"

typedef char* elem_t;

const int MAX_PATH_SIZE     = 50;
const int SIZE_DOT_FILENAME = 60;
const int SIZE_PNG_FILENAME = 60;
const int SIZE_COMMAND      = 120;
const int SIZE_FILENAME     = 20;

typedef struct Node_t {
    elem_t data;
    struct Node_t* left;
    struct Node_t* right;
} Node_t;

typedef struct {
    char* path[MAX_PATH_SIZE];
    int number_of_nodes;
} Path;

Node_t* create_node(elem_t value);
Tree_errors insert_node(Node_t** root, elem_t value);
Node_t* search_node(Node_t* root, elem_t value);
Tree_errors free_tree(Node_t** node);
Tree_errors dump_tree(Node_t* root, FILE* file);
int generate_dot(Node_t* root);
Tree_errors traverse(Node_t* root);
Node_t* build_tree(Node_t* root, char** string_buffer, size_t* line_number, size_t number_of_string);
Tree_errors saveTree(Node_t* node, FILE *file);
char* remove_question_mark(char* str);