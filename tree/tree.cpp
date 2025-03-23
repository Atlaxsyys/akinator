#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tree.h"


Node_t* create_node(elem_t value)
{
    Node_t* node = (Node_t*) calloc(1, sizeof(Node_t));

    if (node == nullptr)   { return nullptr; }

    node->value = value;
    node->left = nullptr;
    node->right = nullptr;

    return node;
}

// Tree_errors insert_node(Node_t** root, elem_t value)
// {
//     if (*root == nullptr)   {create_node(value); }

//     if (strcmp(value, (*root)->value) < 0) { return insert_node(&((*root)->left), value);  }
//     else                                   { return insert_node(&((*root)->right), value); }

//     return SUCCESS;
// }

Node_t* search_node(Node_t* root, elem_t value)
{
    if (root == nullptr)  { return nullptr; }
    
    if (strcmp(root->value, value) == 0) { return root; }

    if (strcmp(value, root->value) < 0)  { return search_node(root->left, value);  }
    else                                 { return search_node(root->right, value); }

    return nullptr;
}

Tree_errors free_tree(Node_t** node)
{
    if (*node == nullptr) { return SUCCESS; }

    free_tree(&((*node)->left));
    free_tree(&((*node)->right));
    // free((*node)->value);
    free(*node);
    *node = nullptr;

    return SUCCESS;
}

Tree_errors dump_tree(Node_t* root, FILE* file)
{
    if(root) fprintf(file, "    \"%p\" [shape=Mrecord, style=filled; fillcolor=\"#F0C0F0\"; label=\"{value: %s | current: %p | { Left: %p | Right: %p } }\"];\n", root, root->value, root, root->left, root->right);

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

    sprintf(dot_filename, "../tests/graph_%d.dot", file_counter);
    sprintf(png_filename, "../tests/graph_%d.png", file_counter);

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

Node_t* delete_node(Node_t* root, elem_t value)
{
    if (!root) { return nullptr; }
    
    if (strcmp(value, root->value) < 0)
    {
        root->left = delete_node(root->left, value);
    }

    else if (strcmp(value, root->value) > 0)
    {
        root->right = delete_node(root->right, value);
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

    fprintf(stderr, "value: \"%s\"\n", root->value);

    if (root->right) { traverse(root->right); }

    return SUCCESS;
}

Node_t* build_tree(Node_t* root, const char** string_buffer, int* line_number, int number_of_string)
{
    assert(string_buffer);

    if (*line_number >= number_of_string) { return nullptr; }

    if(! root)
    {
        root = create_node(string_buffer[*line_number]);
    }

    (*line_number)++;

    if (root->value[strlen(root->value) - 1] == '?')
    {
        root->left  = build_tree(root->left,  string_buffer, line_number, number_of_string);
        root->right = build_tree(root->right, string_buffer, line_number, number_of_string);
    }

    return root;
}   

Tree_errors akinator(Node_t* root)
{
    Node_t* current = root;

    char answer[10] = {};

    fprintf(stderr, "Let's play bro\n");

    while(current)
    {
        fprintf(stderr, "%s (y/n): ", current->value);
        scanf("%s", answer);

        if (strcmp(answer, "y") == 0)
        {
            if (current->left == nullptr)
            {
                fprintf(stderr, "I guessed right\n");

                return SUCCESS;
            }

            current = current->left;
        }

        else if (strcmp(answer, "n") == 0)
        {
            if (current->right == nullptr)
            {
                fprintf(stderr, "I dont know what it is\n");

                return SUCCESS;
            }

            current = current->right;
        }
    }

    return SUCCESS;
}