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
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Akinator SFML");
    // sf::Font font;
    // font.loadFromFile("Tykewriter.ttf");

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

    // while (window.isOpen())
    // {
    //     int selectedOption = -1;
    //     renderMenu(window, font, selectedOption);

    //     switch (selectedOption)
    //     {
    //         case PLAY:
    //         {
    //             Node_t* currentNode = root;
    //             renderAkinator(window, currentNode, font);
    //             break;
    //         }
    //         case SHOW_DATA_BASE:
    //         {
    //             // show_data_base();
    //             break;
    //         }
    //         case COMPARE_NODES:
    //         {
    //             renderCompareNodes(window, root, font);
    //             break;

    //         }
    //         case EXIT_WITH_SAVING: 
    //         {
    //             FILE* file_write = fopen(argv[1], "wb");

    //             if (file_write)
    //             {
    //                 saveTree(root, file_write);
    //                 fclose(file_write);
    //             }

    //             window.close();
    //             break;
    //         }

    //         case EXIT_WITHOUT_SAVING:
    //         {
    //             window.close();
    //             break;
    //         }

    //         default:
    //             break;
    //     }
    // }
    
    const char* FILENAME_DATA_BASE = argv[1];

    menu(root, FILENAME_DATA_BASE);

    generate_dot(root);
    
    free_tree(&root);
    free(text_buffer);
    free(string_buffer);
    
    return 0;
}