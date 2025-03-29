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

    FILE* file_read = fopen(argv[1], "r");

    if (! file_read)
    {
        fprintf(stderr, "Error open file: %s", argv[1]);

        return 1;
    }

    char* text_buffer       = create_buffer(file_read);
    long size_file          = size_text_file(file_read);
    size_t number_of_string = n_string(text_buffer, size_file);

    char** string_buffer = create_string_buffer(text_buffer, file_read);

    fclose(file_read);

    size_t line_number = 0;

    Node_t* root = nullptr;

    root = build_tree(root, string_buffer, &line_number, number_of_string); 

    generate_dot(root);
        
    menu(root, argv);

    generate_dot(root);
    
    free_tree(&root);
    free(text_buffer);
    free(string_buffer);
    
    return 0;
}
