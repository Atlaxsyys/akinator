#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "colour.h"
#include "akinator_game.h"

#ifdef DEBUG_AKINATOR
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

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

void menu(Node_t* root, const char* argv[])
{
    int choise = 0;

    do
    {
        fprintf(stderr, PURPLE_TEXT("------ Welcome to the Akinator game ------\n"));
        fprintf(stderr, GREEN_TEXT("%d. Play\n"), PLAY);
        fprintf(stderr, GREEN_TEXT("%d. Show data_base\n"), SHOW_DATA_BASE);
        fprintf(stderr, GREEN_TEXT("%d. Compare_elements\n"), COMPARE_NODES);
        fprintf(stderr, GREEN_TEXT("%d. Exit with saving\n"), EXIT_WITH_SAVING);
        fprintf(stderr, GREEN_TEXT("%d. Exit without saving\n"), EXIT_WITHOUT_SAVING);
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

            case COMPARE_NODES:
            {
                fprintf(stderr, "enter two elements to compare\n");

                char first_elem[SIZE_ANSWER]  = {};
                char second_elem[SIZE_ANSWER] = {};

                scanf("%s %s", first_elem, second_elem);

                compare_nodes(root, first_elem, second_elem);

                break;
            }

            case EXIT_WITH_SAVING:
            {
                FILE* file_write = fopen(argv[1], "wb");

                saveTree(root, file_write);

                fclose(file_write);

                fprintf(stderr, PURPLE_TEXT("Exit - new data_base saved, goodbye bro!\n"));
                break;
            }

            case EXIT_WITHOUT_SAVING:
            {
                fprintf(stderr, PURPLE_TEXT("Exit - new data_base didn`t save, goodbye bro!\n"));
                break;
            }

            default:
                fprintf(stderr, RED_TEXT("___Unknown command___\n\n"));
                
        }

    } while (choise != EXIT_WITH_SAVING && choise != EXIT_WITHOUT_SAVING);
    
}

void show_data_base()
{
    system("wslview ../graph_dump/graph_0.png");
}

Tree_errors build_path(Node_t* root, Node_t* node, Path* pth)
{
    pth->path[pth->number_of_nodes] = strdup(root->data);
    pth->number_of_nodes++;

    if (root == node)   { return SUCCESS; }

    fprintf (stderr, "node = [%p]\nroot = [%p]\n", node, root);
    Node_t* next_node = search_node (root->left, node->data);

    if (next_node)
    {
        if (build_path(root->left, node, pth) == SUCCESS)
        {
            return SUCCESS;
        }
    }

    next_node = search_node(root->right, node->data);

    if (next_node)
    {
        if (build_path(root->right, node, pth) == SUCCESS)
        {
            return SUCCESS;
        }
    }

    return SUCCESS;
}

Tree_errors compare_nodes(Node_t* root, char* first_data, char* second_data)
{
    Node_t* first_node = search_node(root,  first_data);

    Node_t* second_node = search_node(root, second_data);


    Path first_path  = {};
    Path second_path = {};

    build_path(root, first_node,  &first_path );
    build_path(root, second_node, &second_path);

    int common_part = 0;

    while (common_part < first_path.number_of_nodes &&
          common_part < second_path.number_of_nodes &&
          strcmp(first_path.path[common_part], second_path.path[common_part]) == 0)

          {
              common_part++;
          }

    if (common_part > 0)
    {
        fprintf(stderr, "common characteristic: \n");

        for (int i = 0; i < common_part; i++)   { fprintf(stderr, "- %s\n", first_path.path[i]); }
    }

    if (common_part < first_path.number_of_nodes)
    {
        fprintf(stderr, "unique to %s:\n", first_data);

        for (int i = common_part; i < first_path.number_of_nodes; i++)   { fprintf(stderr, "- %s\n", first_path.path[i]); }
    }

    if (common_part < second_path.number_of_nodes)
    {
        fprintf(stderr, "unique to %s:\n", second_data);

        for (int i = common_part; i < second_path.number_of_nodes; i++)   { fprintf(stderr, "- %s\n", second_path.path[i]); }
    }

    for (int i = 0; i < first_path.number_of_nodes ; i++) free(first_path.path[i] );
    for (int i = 0; i < second_path.number_of_nodes; i++) free(second_path.path[i]);

    return SUCCESS;
}