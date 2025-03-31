#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

#include "tree.h"
#include "file_data.h"
#include "akinator_game.h"
#include "akinator_game_graphic.h"

int main(const int argc, const char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: not enough args:\n %s <txt_file>", argv[0]);

        return NOT_ENOUGH_ARGC;
    }

    FILE* file_read = fopen(argv[1], "r");

    if (! file_read)
    {
        ERROR_MESSAGE(FILE_OPEN_ERR)

        return FILE_OPEN_ERR;
    }

    char* text_buffer       = create_buffer(file_read);
    long size_file          = size_text_file(file_read);
    size_t number_of_string = n_string(text_buffer, size_file);

    char** string_buffer = create_string_buffer(text_buffer, file_read);

    if (fclose(file_read))  ERROR_MESSAGE(FILE_CLOSE_ERR) 

    size_t line_number = 0;

    Node_t* root = build_tree(root, string_buffer, &line_number, number_of_string);

    if(! root)
    {
        ERROR_MESSAGE(NULLPTR_ERR)
        
        return NULLPTR_ERR;
    }

    generate_dot(root); //REVIEW - 

    const char* FILENAME_DATA_BASE = argv[1];

    menu(root, FILENAME_DATA_BASE);

    generate_dot(root);
    
    free_tree(&root);
    free(text_buffer);
    free(string_buffer);
    
    return 0;
}