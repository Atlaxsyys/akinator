#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "tree.h"
#include "colour.h"

#ifdef DEBUG_AKINATOR
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

Node_t* create_node(elem_t data)
{
    Node_t* node = (Node_t*) calloc(1, sizeof(Node_t));
    if (! node)
    {
        ERROR_MESSAGE(MEMORY_ALLOC_ERR)

        return nullptr; 
    }

    node->data = strdup(data);
    if(! node->data) {
        ERROR_MESSAGE(MEMORY_ALLOC_ERR) }

    node->left = nullptr;
    node->right = nullptr;

    return node;
}

Node_t* search_node(Node_t* root, elem_t data)
{
    ON_DEBUG( (stderr, "search: root = [%p]\n", root); )

    ON_DEBUG( (stderr, "after <root==nullptr> : root[%p] >> root-data = '%s' << look for: '%s'\n", root, root->data, data); )

    if (strcmp(root->data, data) == 0)
    {
        ON_DEBUG( (stderr, PURPLE_TEXT("\nI GOT SUKA EGO >>> [%p]: '%s'\n\n"),
                            root, root->data); )
        return root;
    }

    if (root->left)
    {
        Node_t* root_l = search_node(root->left, data);  

        if (root_l != nullptr)  { return root_l; }
    }
    
    if (root->right)
    {
        Node_t* root_r = search_node(root->right, data);

        if (root_r != nullptr)  { return root_r; }
    }

    return nullptr;
}

Tree_errors free_tree(Node_t** node)
{
    if (*node == nullptr) { return SUCCESS; }

    free_tree(&((*node)->left));
    free_tree(&((*node)->right));

    free((*node)->data);
    free(*node);

    *node = nullptr;

    return SUCCESS;
}

Tree_errors dump_tree(Node_t* root, FILE* file)
{
    assert(root);
    
    if (root) 
    fprintf(file, 
            "    \"%p\" [shape=Mrecord, style=filled; fillcolor=\"#F0C0F0\"; label=\"{"
            "data: %s | "
            "current: %p | "
            "{ Left: %p | "
            "Right: %p }"
            "}\"];\n",
            root, 
            root->data, 
            root, 
            root->left, 
            root->right);

    if (root->left)
    {
        fprintf(file, "    \"%p\" -> \"%p\";\n", root, root->left);
        dump_tree(root->left, file);
    }

    if (root->right)
    {
        fprintf(file, "    \"%p\" -> \"%p\";\n", root, root->right);
        dump_tree(root->right, file);
    }

    return SUCCESS;
}

int generate_dot(Node_t* root)
{
    assert(root);

    static int file_counter = 0;

    char* dot_filename = (char*) calloc(SIZE_DOT_FILENAME, sizeof(char));
    if (! dot_filename) {
        ERROR_MESSAGE(MEMORY_ALLOC_ERR) }

    char* png_filename = (char*) calloc(SIZE_PNG_FILENAME, sizeof(char));
    if(! png_filename) {
        ERROR_MESSAGE(MEMORY_ALLOC_ERR) }

    char* command      = (char*) calloc(SIZE_COMMAND,      sizeof(char));
    if(! command) {
        ERROR_MESSAGE(MEMORY_ALLOC_ERR) }

    int written_first  = snprintf(dot_filename, SIZE_DOT_FILENAME, "../graph_dump/graph_%d.dot", file_counter);
    int written_second = snprintf(png_filename, SIZE_PNG_FILENAME, "../graph_dump/graph_%d.png", file_counter);

    if (written_first <  0) ERROR_MESSAGE(SNPRINTF_ERR)
    if (written_second < 0) ERROR_MESSAGE(SNPRINTF_ERR)

    FILE* file = fopen(dot_filename, "w");
    if(! file) {
        ERROR_MESSAGE(FILE_OPEN_ERR) }
    
    fprintf(file, "digraph BinaryTree {\n");
    fprintf(file, "    bgcolor=\"#C0C0C0\";\n\n");
    fprintf(file, "    node [shape=record];\n");
    
    dump_tree(root, file);
    
    fprintf(file, "}\n");

    if(fclose(file) != 0) {
        ERROR_MESSAGE(FILE_CLOSE_ERR) }

    int written_third = snprintf(command, SIZE_COMMAND, "dot -Tpng %s -o %s", dot_filename, png_filename);
    if (written_third < 0) {
        ERROR_MESSAGE(SNPRINTF_ERR) }

    system(command);    

    file_counter++;
    
    free(dot_filename);
    free(png_filename);
    free(command);

    return (file_counter - 1);
}
