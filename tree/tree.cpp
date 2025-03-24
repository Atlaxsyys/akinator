#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tree.h"
#include "colour.h"


Node_t* create_node(elem_t data)
{
    Node_t* node = (Node_t*) calloc(1, sizeof(Node_t));

    if (node == nullptr)   { return nullptr; }

    node->data = strdup(data);
    node->left = nullptr;
    node->right = nullptr;

    return node;
}

Node_t* search_node(Node_t* root, elem_t data)
{
    if (root == nullptr)  { return nullptr; }
    
    if (strcmp(root->data, data) == 0) { return root; }

    if (strcmp(data, root->data) < 0)  { return search_node(root->left, data);  }
    else                                 { return search_node(root->right, data); }

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
    if(root) fprintf(file, "    \"%p\" [shape=Mrecord, style=filled; fillcolor=\"#F0C0F0\"; label=\"{data: %s | current: %p | { Left: %p | Right: %p } }\"];\n", root, root->data, root, root->left, root->right);

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

Tree_errors generate_dot(Node_t* root)
{
    static int file_counter = 0;

    char* dot_filename = (char*) calloc(SIZE_DOT_FILENAME, sizeof(char));
    char* png_filename = (char*) calloc(SIZE_PNG_FILENAME, sizeof(char));
    char* command      = (char*) calloc(SIZE_COMMAND,      sizeof(char));

    sprintf(dot_filename, "../graph_dump/graph_%d.dot", file_counter);
    sprintf(png_filename, "../graph_dump/graph_%d.png", file_counter);

    FILE* file = fopen(dot_filename, "w");
    
    fprintf(file, "digraph BinaryTree {\n");
    fprintf(file, "    bgcolor=\"#C0C0C0\";\n\n");
    fprintf(file, "    node [shape=record];\n");
    
    dump_tree(root, file);
    
    fprintf(file, "}\n");
    fclose(file);

    sprintf(command, "dot -Tpng %s -o %s", dot_filename, png_filename);
    system(command);    

    file_counter++;
    
    free(dot_filename);
    free(png_filename);
    free(command);

    return SUCCESS;
}

Node_t* delete_node(Node_t* root, elem_t data)
{
    if (!root) { return nullptr; }
    
    if (strcmp(data, root->data) < 0)
    {
        root->left = delete_node(root->left, data);
    }

    else if (strcmp(data, root->data) > 0)
    {
        root->right = delete_node(root->right, data);
    }

    else 
    {
        free_tree(&root);

        return nullptr;
    }

    return root;
}

Tree_errors traverse(Node_t* root)
{
    if (! root) { return SUCCESS; }

    if (root->left) { traverse(root->left); }

    fprintf(stderr, "data: \"%s\"\n", root->data);

    if (root->right) { traverse(root->right); }

    return SUCCESS;
}

Node_t* build_tree(Node_t* root, char** string_buffer, int* line_number, size_t number_of_string)
{
    assert(string_buffer);

    if (*line_number >= number_of_string) { return nullptr; }

    if(! root)  { root = create_node(string_buffer[*line_number]); }

    (*line_number)++;

    if (root->data[strlen(root->data) - 1] == '?')
    {
        root->left  = build_tree(root->left,  string_buffer, line_number, number_of_string);
        root->right = build_tree(root->right, string_buffer, line_number, number_of_string);
    }

    return root;
}   

Tree_errors saveTree(Node_t* root, FILE *file)
{
    if(! root)
    {
        return SUCCESS;
    }

    fprintf(file, "%s\n", root->data);

    saveTree(root->left, file);
    saveTree(root->right, file);

    return SUCCESS;
}