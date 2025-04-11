#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "colour.h"
#include "akinator_game.h"
#include "errors.h"
#include "logger.h"

#ifdef DEBUG_AKINATOR
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

static void show_data_base(Node_t* root);
static void clean_buffer();
static void game_mode(Node_t* root, const char* filename_data_base, int choise);
static void output_definition(Node_t* root, Path path, char data[]);
static Tree_errors definition_node(Node_t* root);
static Tree_errors saveTree(Node_t* root, FILE *file, int level);
static Tree_errors akinator(Node_t* root);
static Tree_errors add_new_node(Node_t* current);
static Tree_errors build_path(Node_t* root, Node_t* node, Path* pth);
static Tree_errors compare_nodes(Node_t* root);
static int get_common_part(Path first_path, Path second_path);
static Tree_errors check_node_exists(Node_t* root, Node_t** node, const char* promts, char elem[]);
static Tree_errors output_unique_features(Node_t* root, int common_part, char first_elem[], Path path);
static Tree_errors output_common_features(int common_part, Path path);
static Tree_errors exit_with_saving(Node_t* root,const char* filename_data_base);
static char* remove_question_mark(char* str);

void clean_buffer()
{
    int some_char = 0;
    while ((some_char = getchar()) != '\n' && some_char != EOF);
}

Tree_errors akinator(Node_t* root)
{
    assert(root);

    Node_t* current = root;

    char answer[SIZE_ANSWER] = {};

    fprintf(stderr, PURPLE_TEXT("Let's play bro\n"));

    while (current)
    {
        fprintf(stderr, YELLOW_TEXT("%s (y/n): "), current->data);
        scanf("%1s", answer);

        clean_buffer();

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
                fprintf(stderr, PURPLE_TEXT("I dont know what it is.\nYou want add new element? (y/n): "));
                scanf("%1s", answer);

                while(strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0)
                {
                    fprintf(stderr, RED_TEXT("Please, enter y or n\n"));
                    
                    clean_buffer();

                    fprintf(stderr, PURPLE_TEXT("You want add new element? (y/n): "));
                    scanf("%1s", answer);
                }

                clean_buffer();

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

Tree_errors add_new_node(Node_t* current)
{
    assert(current);

    char new_answer[SIZE_ANSWER]     = {};
    char new_question[SIZE_QUESTION] = {}; 

    fprintf(stderr, YELLOW_TEXT("What word did you wish for?: "));

    char* fgets_err_first = fgets(new_answer, sizeof(new_answer), stdin);
    if(! fgets_err_first) {
        LOG_ERROR("fgets error"); }

    new_answer[strlen(new_answer) - 1] = '\0';

    fprintf(stderr, YELLOW_TEXT("Which question differs %s from %s: "), new_answer, current->data);

    char* fgets_err_second = fgets(new_question, sizeof(new_question), stdin);
    if (! fgets_err_second) {
        LOG_ERROR("fgets error"); }

    new_question[strlen(new_question) - 1] = '\0';

    char* old_answer = strdup(current->data);
    if (! old_answer) {
        LOG_ERROR("old_answer = nullptr"); }

    free(current->data);

    current->data = strdup(new_question);
    if (! current->data) {
        LOG_ERROR("current->data = nullptr"); }

    current->left  = create_node(new_answer);
    current->right = create_node(old_answer);

    free(old_answer);

    return SUCCESS;
}

void menu(Node_t* root, const char* filename_data_base)
{
    assert(filename_data_base);
    assert(root);

    int choise = 0;

    do
    {
        fprintf(stderr, PURPLE_TEXT("------ Welcome to the Akinator game ------\n"));
        fprintf(stderr, GREEN_TEXT("%d. Play\n"), PLAY);
        fprintf(stderr, GREEN_TEXT("%d. Show database\n"), SHOW_DATA_BASE);
        fprintf(stderr, GREEN_TEXT("%d. definition\n"), DEFINITION);
        fprintf(stderr, GREEN_TEXT("%d. Compare elements\n"), COMPARE_NODES);
        fprintf(stderr, GREEN_TEXT("%d. Exit with saving\n"), EXIT_WITH_SAVING);
        fprintf(stderr, GREEN_TEXT("%d. Exit without saving\n"), EXIT_WITHOUT_SAVING);
        fprintf(stderr, LIGHT_BLUE_TEXT("choose an action\n"));

        scanf("%d", &choise);
        
        clean_buffer();

        game_mode(root, filename_data_base, choise);

    } while (choise != EXIT_WITH_SAVING && choise != EXIT_WITHOUT_SAVING);
}

void game_mode(Node_t* root, const char* filename_data_base, int choise)
{
    assert(root);
    assert(filename_data_base);

    switch (choise)
        { 
            case PLAY:
            {
                akinator(root);
                break;
            }

            case SHOW_DATA_BASE:
            {
                show_data_base(root);
                break;
            }

            case DEFINITION:
            {
                definition_node(root);
                break;
            }

            case COMPARE_NODES:
            {
                compare_nodes(root);
                break;
            }

            case EXIT_WITH_SAVING:
            {
                exit_with_saving(root, filename_data_base);
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
}

Tree_errors exit_with_saving(Node_t* root, const char* filename_data_base)
{
    assert(root);

    FILE* file_write = fopen(filename_data_base, "wb");
    if (! file_write) { 
        LOG_ERROR("file opening error: %s", filename_data_base); }
    
    int level = 0;
    saveTree(root, file_write, level);

    if (fclose(file_write) != 0) {
        LOG_ERROR("file closing error: %s", filename_data_base); }
    fprintf(stderr, PURPLE_TEXT("Exit - new data_base saved, goodbye bro!\n"));

    return SUCCESS;
}

void show_data_base(Node_t* root)
{
    int number_of_file = generate_dot(root);
    
    char command[SIZE_DOT_FILENAME] = {};
    
    int written = snprintf(command, sizeof(command), "wslview ../resources/graph_dump/graph_%d.png", number_of_file);
    if (written < 0) {
        LOG_ERROR("snprintf error"); }
    
    system(command);
}

Tree_errors build_path(Node_t* root, Node_t* node, Path* pth)
{   
    assert(root);

    pth->path[pth->number_of_nodes] = strdup(root->data);
    if(! pth->path[pth->number_of_nodes])
    {
        LOG_ERROR(RED_TEXT("pth->path[pth->number_of_nodes] = nullptr"));
        
        return NULLPTR_ERR;
    }

    pth->number_of_nodes++; 

    if (root == node)   { return SUCCESS; }

    ON_DEBUG( fprintf (stderr, "node = [%p]\nroot = [%p]\n", node, root); )

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

char* remove_question_mark(char* str)
{
    assert(str);

    size_t len = strlen(str);
    
    if (len > 0 && str[len - 1] == '?')
    {
        str[len - 1] = '\0';
    }

    return str;
};

Tree_errors definition_node(Node_t* root)
{
    assert(root);

    Node_t* node = nullptr;

    char data[SIZE_ANSWER] = {};

    check_node_exists(root, &node, nullptr, data);

    Path path = {};

    Tree_errors err = build_path(root, node, &path);
    if(! err)   LOG_ERROR("build_path error");

    output_definition(root, path, data);

    for (int i = 0; i < path.number_of_nodes ; i++) free(path.path[i] );

    return SUCCESS;
}

void output_definition(Node_t* root, Path path, char data[])
{
    assert(root);
    assert(data);

    for (int i = 0; i < path.number_of_nodes; i++)
    {
        if (i > 0)
        {
            Node_t* parent = search_node(root, path.path[i - 1]);

            if (parent && parent->right && strcmp(parent->right->data, path.path[i]) == 0)
            {
                fprintf(stderr, LIGHT_BLUE_TEXT("- No %s\n"), remove_question_mark( path.path[i - 1]));
            }
            
            else
            {
                fprintf(stderr, LIGHT_BLUE_TEXT("- %s\n"), remove_question_mark( path.path[i - 1]));
            }
        }
    }
}

Tree_errors compare_nodes(Node_t* root)
{
    assert(root);

    Node_t* first_node = nullptr;
    Node_t* second_node = nullptr;

    char first_elem[SIZE_ANSWER]  = {};
    char second_elem[SIZE_ANSWER] = {};

    const char* prompts[] = {"first", "second"};

    check_node_exists(root, &first_node, prompts[0], first_elem);
    check_node_exists(root, &second_node, prompts[1], second_elem);

    Path first_path  = {};
    Path second_path = {};

    Tree_errors err_first  = build_path(root, first_node,  &first_path );
    if (! err_first)    LOG_ERROR("first build path error");

    Tree_errors err_second = build_path(root, second_node, &second_path);
    if (! err_second)    LOG_ERROR("second build path error");

    LOG_DEBUG("------------------   paths ------------------");
    for(int i = 0; i < first_path.number_of_nodes;  i++) LOG_DEBUG("first_path[%d]: %s", i, first_path.path[i] );
    for(int i = 0; i < second_path.number_of_nodes; i++) LOG_DEBUG("second_path[%d]: %s", i, second_path.path[i]);
    LOG_DEBUG("-----------------------------------------------");


    int common_part = get_common_part(first_path, second_path);

    LOG_DEBUG("common_part of paths: %d", common_part);

    output_common_features(common_part, first_path);

    if (common_part < first_path.number_of_nodes )    output_unique_features(root, common_part, first_elem, first_path);
    if (common_part < second_path.number_of_nodes)    output_unique_features(root, common_part, second_elem, second_path);

    for (int i = 0; i < first_path.number_of_nodes ; i++) free(first_path.path[i] );
    for (int i = 0; i < second_path.number_of_nodes; i++) free(second_path.path[i]);

    return SUCCESS;
}

Tree_errors check_node_exists(Node_t* root, Node_t** node, const char* promts, char elem[])
{
    assert(root);
    assert(node);
    assert(elem);

    do
    {
        if (promts == nullptr)   fprintf(stderr, YELLOW_TEXT("enter element: "));
        else                     fprintf(stderr, YELLOW_TEXT("enter %s element: "), promts);

        scanf("%s", elem);

        clean_buffer();

        (*node) = search_node(root, elem);

        if ((*node) == nullptr) { fprintf(stderr, RED_TEXT("Element '%s' not found. Please, try again.\n"), elem); }

    } while ((*node) == nullptr);

    return SUCCESS;
}

int get_common_part(Path first_path, Path second_path)
{
    int common_part = 0;

    while (common_part < first_path.number_of_nodes  &&
           common_part < second_path.number_of_nodes &&
          strcmp(first_path.path[common_part], second_path.path[common_part]) == 0)
          {
              common_part++;
          }
    
    return common_part;
}

Tree_errors output_unique_features(Node_t* root, int common_part, char first_elem[], Path path)
{
    assert(root);
    assert(first_elem);

    fprintf(stderr, BLUE_TEXT("unique to %s:\n"), first_elem);

    for (int i = common_part; i < path.number_of_nodes; i++)
    {
        if (i > 0)
        {
            Node_t* parent = search_node(root, path.path[i - 1]);

            if (parent && parent->right && strcmp(parent->right->data, path.path[i]) == 0)
            {
                fprintf(stderr, LIGHT_BLUE_TEXT("- No %s\n"), remove_question_mark( path.path[i - 1]));
            }
            
            else
            {
                fprintf(stderr, LIGHT_BLUE_TEXT("- %s\n"), remove_question_mark( path.path[i - 1]));
            }
        }
    }

    return SUCCESS;
}

Tree_errors output_common_features(int common_part, Path path)
{
    fprintf(stderr, BLUE_TEXT("common features: \n"));

    if (common_part == 1)
    {
        fprintf(stderr, LIGHT_BLUE_TEXT("nothing\n"));
    }

    if (common_part > 0)
    {
        for (int i = 1; i < common_part; i++)
        {
            fprintf(stderr, LIGHT_BLUE_TEXT("- %s\n"), remove_question_mark( path.path[i]));
        }
    }

    return SUCCESS;
}

Node_t* build_tree(Node_t* root, char** string_buffer, size_t* line_number, size_t number_of_string)
{
    assert(string_buffer);

    if (*line_number >= number_of_string) { return nullptr; }

    char* current_string = string_buffer[*line_number];
    while (*current_string == ' ' || *current_string == '\t')
    {
        current_string++;
    }

    if (*current_string == '\0')
    {
        (*line_number)++;
        return build_tree(root, string_buffer, line_number, number_of_string);
    }

    if (!root)
    {
        root = create_node(current_string);
    }

    (*line_number)++;

    if (root->data[strlen(root->data) - 1] == '?')
    {
        root->left  = build_tree(root->left,  string_buffer, line_number, number_of_string);
        root->right = build_tree(root->right, string_buffer, line_number, number_of_string);
    }

    return root;
}

Tree_errors saveTree(Node_t* root, FILE *file, int level)
{
    if (!root)
    {
        return SUCCESS;
    }

    for (int i = 0; i < level; i++)
    {
        fprintf(file, "    ");
    }

    fprintf(file, "%s\n", root->data);

    saveTree(root->left, file, level + 1);
    saveTree(root->right, file, level + 1);

    return SUCCESS;
}