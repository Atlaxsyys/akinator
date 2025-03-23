#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "file_data.h"
#include "akinator_game.h"

int main(const int argc, const char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: not enough args:\n %s <txt_file>", argv[0]);

        return 1;
    }

    FILE* file_read = fopen(argv[1], "rw");

    if (! file_read)
    {
        fprintf(stderr, "Error open file: %s", argv[1]);

        return 1;
    }

    char* text_buffer   = create_buffer(file_read);
    long size_file = size_text_file(file_read);
    size_t number_of_string = n_string(text_buffer, size_file);

    char** string_buffer = create_string_buffer(text_buffer, file_read);

    fclose(file_read);

    int line_number = 0;

    Node_t* root = nullptr;

    root = build_tree(root, string_buffer, &line_number, number_of_string); 

    generate_dot(root);
    game(root);
    generate_dot(root);

    FILE* file_write = fopen(argv[1], "wb");

    if (! file_write)
    {
        fprintf(stderr, "Error open file: %s", argv[1]);

        return 1;
    }

    saveTree(root, file_write);

    fclose(file_write);

    free_tree(&root);
    free(text_buffer);
    free(string_buffer);
    
    return 0;
}
