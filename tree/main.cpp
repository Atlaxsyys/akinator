#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "file_data.h"

int main()
{
    FILE* file_read = fopen(FILE_NAME, "rw");

    char* text_buffer   = create_buffer(file_read);
    long size_file = size_text_file(file_read);
    size_t number_of_string = n_string(text_buffer, size_file);

    const char** string_buffer = create_string_buffer(text_buffer, file_read);

    fclose(file_read);

    int line_number = 0;

    Node_t* root = nullptr;

    root = build_tree(root, string_buffer, &line_number, number_of_string); 

    generate_dot(root);
    fprintf(stderr, "\nDAMMRR\n");
    akinator(root);

    free_tree(&root);
    free(text_buffer);
    free(string_buffer);
    
    return 0;
}
