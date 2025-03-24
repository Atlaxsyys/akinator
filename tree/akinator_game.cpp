#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "colour.h"
#include "akinator_game.h"

Tree_errors akinator(Node_t* root)
{
    Node_t* current = root;

    char answer[SIZE_ANSWER] = {};

    fprintf(stderr, PURPLE_TEXT("Let's play bro\n"));

    while(current)
    {
        fprintf(stderr, YELLOW_TEXT("%s (y/n): "), current->data);
        scanf("%s", answer);

        if (strcmp(answer, "y") == 0)
        {
            if (current->left == nullptr)
            {
                fprintf(stderr, GREEN_TEXT("I guessed right\n"));

                return SUCCESS;
            }

            current = current->left;
        }

        else if (strcmp(answer, "n") == 0)
        {
            if (current->right == nullptr)
            {
                fprintf(stderr, PURPLE_TEXT("I dont know what it is.\nYou want add new element (y/n): "));
                scanf("%s", answer);

                if (strcmp(answer, "y") == 0)
                {
                    add_new_node(current);
                }

                return SUCCESS;
            }

            current = current->right;
        }

        else
        {
            fprintf(stderr, RED_TEXT("Please, enter y or n\n"));
        }
    }

    return SUCCESS;
}

Tree_errors game(Node_t* root)
{
    char play_again[SIZE_ANSWER] = {};

    do
    {
        akinator(root);

        fprintf(stderr, LIGHT_BLUE_TEXT("You want play again? (y/n): "));
        scanf("%s", play_again);

    } while(strcmp(play_again, "y") == 0);

    return SUCCESS;
}

Tree_errors add_new_node(Node_t* current)
{
    char new_answer[SIZE_ANSWER]     = {};
    char new_question[SIZE_QUESTION] = {}; 

    fprintf(stderr, YELLOW_TEXT("What word did you wish for?"));

    getchar();

    fgets(new_answer, sizeof(new_answer), stdin);

    new_answer[strlen(new_answer) - 1] = '\0';

    fprintf(stderr, YELLOW_TEXT("Which question differs %s from %s"), new_answer, current->data);
    fgets(new_question, sizeof(new_question), stdin);

    new_question[strlen(new_question) - 1] = '\0';

    char* old_answer = strdup(current->data);

    free(current->data);

    current->data = strdup(new_question);

    current->left  = create_node(new_answer);
    current->right = create_node(old_answer);

    free(old_answer);

    return SUCCESS;
}

void menu(Node_t* root)
{
    int choise = 0;

    do
    {
        fprintf(stderr, PURPLE_TEXT("------ Welcome to the Akinator game ------\n"));
        fprintf(stderr, GREEN_TEXT("1. Play\n"));
        fprintf(stderr, GREEN_TEXT("2. Show data_base\n"));
        fprintf(stderr, GREEN_TEXT("3. Exit\n"));
        fprintf(stderr, LIGHT_BLUE_TEXT("choose an action\n"));

        scanf("%d", &choise);
        getchar();

        switch(choise)
        {
            case PLAY:
            {
                akinator(root);
                break;
            }

            case SHOW_DATA_BASE:
            {
                show_data_base();
                break;
            }

            case EXIT:
            {
                fprintf(stderr, PURPLE_TEXT("Exit, goodbye bro!\n"));
                break;
            }

            default:
                fprintf(stderr, RED_TEXT("___Unknown command___\n\n"));
                
        }

    } while (choise != EXIT);
    
}

void show_data_base()
{
    system("wslview ../tests/graph_0.png");
}