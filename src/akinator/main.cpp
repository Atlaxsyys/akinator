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
#include "logger.h"

int main(const int argc, const char* argv[])
{
    const char* log_filename = "../resources/logger/logger.log";
        
    Logger_t* logger = logger_constructor(log_filename, DEBUG);

    LOG_INFO("The program has begun");

    if (argc != 2)
    {
        LOG_ERROR("Error: not enough args:\n %s <txt_file>", argv[0]);

        return NOT_ENOUGH_ARGC;
    }

    FILE* file_read = fopen(argv[1], "r");

    if (! file_read)
    {
        LOG_ERROR("FILE OPEN ERROR");

        return FILE_OPEN_ERR;
    }

    char* text_buffer       = create_buffer(file_read);
    long size_file          = size_text_file(file_read);

    size_t number_of_string = n_string(text_buffer, size_file);
    LOG_DEBUG("number_of_string: %zu", number_of_string);

    char** string_buffer = create_string_buffer(text_buffer, file_read);
    
    for (size_t i = 0; i < number_of_string; i++)  LOG_DEBUG("string_buffer[%d]: %s", i, string_buffer[i]);
    
    if (fclose(file_read)) LOG_ERROR("FILE CLOSE ERROR");

    size_t line_number = 0;

    Node_t* root = build_tree(root, string_buffer, &line_number, number_of_string);

    if(! root)
    {
        LOG_ERROR("root = nullptr!");
        
        return NULLPTR_ERR;
    }

    const char* filename_data_base = argv[1];
    LOG_DEBUG("filename_data_base: %s", filename_data_base);

    menu(root, filename_data_base);
    
    logger_destructor(logger);
    free_tree(&root);
    free(text_buffer);
    free(string_buffer);
    
    return 0;
}