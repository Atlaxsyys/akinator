#pragma once

typedef const char* elem_t;

typedef struct Node_t {
    elem_t value;
    struct Node_t* left;
    struct Node_t* right;
} Node_t;

enum Tree_errors
{
    SUCCESS = 0,
    MEMORY_ALLOC_ERR = 1,
};

const int SIZE_DOT_FILENAME = 60;
const int SIZE_PNG_FILENAME = 60;
const int SIZE_COMMAND      = 120;

Node_t* create_node(elem_t value);
Tree_errors insert_node(Node_t** root, elem_t value);
Node_t* search_node(Node_t* root, elem_t value);
Tree_errors free_tree(Node_t** node);
Tree_errors dump_tree(Node_t* root, FILE* file);
Tree_errors generate_dot(Node_t* root);
Node_t* delete_node(Node_t* root, elem_t value);
Tree_errors traverse(Node_t* root);
Node_t* build_tree(Node_t* root, const char** string_buffer, int* line_number, int number_of_string);
Tree_errors akinator(Node_t* root);