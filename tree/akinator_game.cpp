#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

#include "colour.h"
#include "akinator_game.h"

#ifdef DEBUG_AKINATOR
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

char* remove_question_mark(char* str);

void renderMenu(sf::RenderWindow &window, sf::Font &font, int &selectedOption) {
    // Настройка заголовка
    sf::Text title("------ Welcome to the Akinator game ------", font, 24);
    title.setFillColor(sf::Color::Magenta);
    title.setPosition(100, 50);

    // Настройка пунктов меню
    std::string options[] = {"Play", "Show Database", "Compare Elements", "Exit with Saving", "Exit without Saving"};
    sf::Text menuOptions[5];
    sf::RectangleShape buttons[5];

    for (int i = 0; i < 5; i++) {
        buttons[i].setSize(sf::Vector2f(300, 40));
        buttons[i].setFillColor(sf::Color(70, 70, 70));
        buttons[i].setOutlineThickness(2);
        buttons[i].setOutlineColor(sf::Color::White);
        buttons[i].setPosition(140, 145 + i * 60);

        menuOptions[i].setFont(font);
        menuOptions[i].setString(options[i]);
        menuOptions[i].setCharacterSize(20);
        menuOptions[i].setFillColor(sf::Color::White);
        menuOptions[i].setPosition(150, 150 + i * 60);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                selectedOption = EXIT_WITHOUT_SAVING;
                return;
            }

            // Обработка нажатия кнопок мыши
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (int i = 0; i < 5; i++) {
                        if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            selectedOption = i + 1; // PLAY=1, SHOW_DATA_BASE=2 и т.д.
                            return;
                        }
                    }
                }
            }

            // Подсветка кнопок при наведении
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < 5; i++) {
                    if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        buttons[i].setFillColor(sf::Color(100, 100, 100));
                    } else {
                        buttons[i].setFillColor(sf::Color(70, 70, 70));
                    }
                }
            }
        }

        // Отрисовка
        window.clear(sf::Color(30, 30, 30));
        window.draw(title);
        for (int i = 0; i < 5; i++) {
            window.draw(buttons[i]);
            window.draw(menuOptions[i]);
        }
        window.display();
    }
}

void renderAkinator(sf::RenderWindow &window, Node_t* &currentNode, sf::Font &font) {
    std::string tempNewAnswer; // Для хранения нового ответа между шагами
    // Настройка текста вопроса
    sf::Text questionText;
    questionText.setFont(font);
    questionText.setCharacterSize(30);
    questionText.setFillColor(sf::Color::White);
    questionText.setPosition(350, 200);
    questionText.setString(currentNode->data);

    // Настройка кнопок Yes/No
    sf::RectangleShape yesButton(sf::Vector2f(150, 50));
    yesButton.setFillColor(sf::Color(50, 150, 50));
    yesButton.setPosition(200, 400);
    yesButton.setOutlineThickness(2);
    yesButton.setOutlineColor(sf::Color::White);

    sf::RectangleShape noButton(sf::Vector2f(150, 50));
    noButton.setFillColor(sf::Color(150, 50, 50));
    noButton.setPosition(450, 400);
    noButton.setOutlineThickness(2);
    noButton.setOutlineColor(sf::Color::White);

    sf::Text yesText("YES", font, 24);
    yesText.setFillColor(sf::Color::White);
    yesText.setPosition(250, 410);

    sf::Text noText("NO", font, 24);
    noText.setFillColor(sf::Color::White);
    noText.setPosition(500, 410);

    // Кнопка возврата в меню
    sf::RectangleShape backButton(sf::Vector2f(100, 40));
    backButton.setFillColor(sf::Color(70, 70, 70));
    backButton.setPosition(20, 20);
    backButton.setOutlineThickness(1);
    backButton.setOutlineColor(sf::Color::White);

    sf::Text backText("Menu", font, 20);
    backText.setFillColor(sf::Color::White);
    backText.setPosition(35, 25);

    // Состояния для обработки угадывания
    enum class GuessState { Questioning, Guessed, AddingNew };
    GuessState guessState = GuessState::Questioning;
    std::string userInput;
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(50, 350);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (guessState == GuessState::Questioning) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Обработка кнопки возврата в меню
                    if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        return;
                    }

                    // Обработка кнопок Yes/No
                    if (yesButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (currentNode->left) {
                            currentNode = currentNode->left;
                            questionText.setString(currentNode->data);
                        } else {
                            // Достигли конечного узла - спрашиваем, правильно ли угадали
                            guessState = GuessState::Guessed;
                            questionText.setString("Is it " + std::string(currentNode->data) + "?");
                        }
                    }
                    else if (noButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (currentNode->right) {
                            currentNode = currentNode->right;
                            questionText.setString(currentNode->data);
                        } else {
                            // Достигли конечного узла - спрашиваем, правильно ли угадали
                            guessState = GuessState::Guessed;
                            questionText.setString("Is it " + std::string(currentNode->data) + "?");
                        }
                    }
                }
            }
            else if (guessState == GuessState::Guessed) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Обработка кнопки возврата в меню
                    if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        return;
                    }

                    // Обработка кнопок Yes/No при подтверждении ответа
                    if (yesButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        // Правильно угадали - возвращаемся в меню
                        return;
                    }
                    else if (noButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        // Не угадали - переходим к добавлению нового элемента
                        guessState = GuessState::AddingNew;
                        questionText.setString("What word did you wish for?");
                        userInput.clear();
                        inputText.setString(userInput);
                    }
                }
            }
            else if (guessState == GuessState::AddingNew) {
                // Обработка текстового ввода
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b') { // Backspace
                        if (!userInput.empty()) {
                            userInput.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\r') {
                        userInput += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(userInput);
                }
        
                // Обработка нажатия Enter
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && !userInput.empty()) {
                    if (questionText.getString().find("What word did you wish for?") != std::string::npos) {
                        // Сохраняем новый ответ
                        std::string newAnswer = userInput;
                        userInput.clear();
                        questionText.setString("Which question differs " + newAnswer + " from " + std::string(currentNode->data) + "?");
                        inputText.setString(userInput);
                        
                        // Сохраняем новый ответ во временную переменную
                        tempNewAnswer = newAnswer;
                    }
                    else {
                        // Создаем новый узел
                        char* old_answer = strdup(currentNode->data);
                        free(currentNode->data);
                        
                        // Копируем вопрос в data узла
                        currentNode->data = strdup(userInput.c_str());
                        
                        // Создаем левый и правый узлы
                        currentNode->left = create_node(strdup(tempNewAnswer.c_str()));
                        currentNode->right = create_node(old_answer);
                        
                        free(old_answer);
                        return;
                    }
                }
            }
        }

        // Отрисовка
        window.clear(sf::Color(30, 30, 30));
        
        // Рисуем вопрос
        window.draw(questionText);
        
        // В зависимости от состояния рисуем разные элементы
        if (guessState == GuessState::Questioning) {
            // Рисуем кнопки Yes/No для навигации по дереву
            window.draw(yesButton);
            window.draw(noButton);
            window.draw(yesText);
            window.draw(noText);
        }
        else if (guessState == GuessState::Guessed) {
            // Рисуем кнопки Yes/No для подтверждения ответа
            window.draw(yesButton);
            window.draw(noButton);
            window.draw(yesText);
            window.draw(noText);
        }
        else if (guessState == GuessState::AddingNew) {
            // Рисуем поле ввода
            window.draw(inputText);
        }
        
        // Всегда рисуем кнопку возврата
        window.draw(backButton);
        window.draw(backText);
        
        window.display();
    }
}

void renderCompareNodes(sf::RenderWindow& window, Node_t* root, sf::Font& font) {
    // Состояния интерфейса
    enum class State { Input, Results };
    State currentState = State::Input;
    
    // Данные для ввода
    std::string firstInput;
    std::string secondInput;
    int activeInputField = 1; // 1 - первое поле, 2 - второе
    size_t cursorPos = 0;
    
    // Данные результатов
    std::vector<std::string> commonFeatures;
    std::vector<std::string> uniqueFirst;
    std::vector<std::string> uniqueSecond;

    // Параметры отображения
    const float initialFontSize = 22.f;
    const float lineHeight = 30.f;
    const float padding = 20.f;
    float startY = 150.f;

    // Элементы UI
    sf::Text title("Compare Elements", font, 30);
    title.setFillColor(sf::Color::Cyan);
    title.setPosition(50.f, 50.f);

    // Поля ввода
    sf::RectangleShape inputField1(sf::Vector2f(400.f, 40.f));
    inputField1.setPosition(50.f, 100.f);
    inputField1.setFillColor(sf::Color(60, 60, 60));
    inputField1.setOutlineThickness(2.f);
    inputField1.setOutlineColor(sf::Color(150, 150, 150));

    sf::RectangleShape inputField2(sf::Vector2f(400.f, 40.f));
    inputField2.setPosition(50.f, 160.f);
    inputField2.setFillColor(sf::Color(60, 60, 60));
    inputField2.setOutlineThickness(2.f);
    inputField2.setOutlineColor(sf::Color(150, 150, 150));

    sf::Text inputLabel1("First element:", font, 24);
    inputLabel1.setPosition(50.f, 70.f);
    
    sf::Text inputLabel2("Second element:", font, 24);
    inputLabel2.setPosition(50.f, 130.f);

    sf::Text inputText1("", font, 24);
    inputText1.setPosition(55.f, 105.f);
    
    sf::Text inputText2("", font, 24);
    inputText2.setPosition(55.f, 165.f);

    // Кнопки
    sf::RectangleShape compareButton(sf::Vector2f(150.f, 40.f));
    compareButton.setPosition(500.f, 130.f);
    compareButton.setFillColor(sf::Color(0, 120, 200));
    
    sf::Text compareButtonText("Compare", font, 24);
    compareButtonText.setPosition(510.f, 135.f);

    sf::RectangleShape backButton(sf::Vector2f(100.f, 40.f));
    backButton.setPosition(20.f, 20.f);
    backButton.setFillColor(sf::Color(70, 70, 70));
    
    sf::Text backButtonText("Back", font, 20);
    backButtonText.setPosition(35.f, 25.f);

    // Курсор
    sf::RectangleShape cursor(sf::Vector2f(2.f, 30.f));
    cursor.setFillColor(sf::Color::White);
    sf::Clock cursorClock;
    bool cursorVisible = true;

    // Главный цикл
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            // Обработка кликов
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (backButton.getGlobalBounds().contains(mousePos)) {
                    return;
                }

                if (currentState == State::Input) {
                    // Выбор активного поля ввода
                    if (inputField1.getGlobalBounds().contains(mousePos)) {
                        activeInputField = 1;
                        cursorPos = firstInput.size();
                        inputField1.setOutlineColor(sf::Color::Cyan);
                        inputField2.setOutlineColor(sf::Color(150, 150, 150));
                    }
                    else if (inputField2.getGlobalBounds().contains(mousePos)) {
                        activeInputField = 2;
                        cursorPos = secondInput.size();
                        inputField1.setOutlineColor(sf::Color(150, 150, 150));
                        inputField2.setOutlineColor(sf::Color::Cyan);
                    }
                    // Нажатие кнопки Compare
                    else if (compareButton.getGlobalBounds().contains(mousePos)) {
                        // Преобразование string в char* для совместимости
                        char firstBuf[SIZE_ANSWER] = {};
                        char secondBuf[SIZE_ANSWER] = {};
                        strncpy(firstBuf, firstInput.c_str(), SIZE_ANSWER-1);
                        strncpy(secondBuf, secondInput.c_str(), SIZE_ANSWER-1);

                        Node_t* firstNode = search_node(root, firstBuf);
                        Node_t* secondNode = search_node(root, secondBuf);

                        if (firstNode && secondNode) {
                            // Очистка предыдущих результатов
                            commonFeatures.clear();
                            uniqueFirst.clear();
                            uniqueSecond.clear();

                            // Получение путей
                            Path firstPath = {}, secondPath = {};
                            build_path(root, firstNode, &firstPath);
                            build_path(root, secondNode, &secondPath);

                            // Поиск общих черт
                            int commonCount = 0;
                            while (commonCount < firstPath.number_of_nodes &&
                                   commonCount < secondPath.number_of_nodes &&
                                   strcmp(firstPath.path[commonCount], secondPath.path[commonCount]) == 0) {
                                commonCount++;
                            }

                            // Заполнение общих черт
                            for (int i = 1; i < commonCount; i++) {
                                commonFeatures.push_back("- " + std::string(remove_question_mark(firstPath.path[i])));
                            }

                            // Заполнение уникальных черт первого элемента
                            for (int i = commonCount; i < firstPath.number_of_nodes; i++) {
                                if (i > 0) {
                                    Node_t* parent = search_node(root, firstPath.path[i-1]);
                                    std::string prefix = (parent && parent->right && 
                                                        strcmp(parent->right->data, firstPath.path[i]) == 0) ? 
                                                        "- No " : "- ";
                                    uniqueFirst.push_back(prefix + std::string(remove_question_mark(firstPath.path[i-1])));
                                }
                            }

                            // Заполнение уникальных черт второго элемента
                            for (int i = commonCount; i < secondPath.number_of_nodes; i++) {
                                if (i > 0) {
                                    Node_t* parent = search_node(root, secondPath.path[i-1]);
                                    std::string prefix = (parent && parent->right && 
                                                        strcmp(parent->right->data, secondPath.path[i]) == 0) ? 
                                                        "- No " : "- ";
                                    uniqueSecond.push_back(prefix + std::string(remove_question_mark(secondPath.path[i-1])));
                                }
                            }

                            currentState = State::Results;
                        }
                    }
                }
            }

            // Обработка ввода текста
            if (currentState == State::Input && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Backspace
                    if (activeInputField == 1 && !firstInput.empty() && cursorPos > 0) {
                        firstInput.erase(--cursorPos, 1);
                        inputText1.setString(firstInput);
                    }
                    else if (activeInputField == 2 && !secondInput.empty() && cursorPos > 0) {
                        secondInput.erase(--cursorPos, 1);
                        inputText2.setString(secondInput);
                    }
                }
                else if (event.text.unicode >= 32 && event.text.unicode < 128) { // Печатные символы
                    if (activeInputField == 1 && firstInput.size() < SIZE_ANSWER-1) {
                        firstInput.insert(cursorPos++, 1, static_cast<char>(event.text.unicode));
                        inputText1.setString(firstInput);
                    }
                    else if (activeInputField == 2 && secondInput.size() < SIZE_ANSWER-1) {
                        secondInput.insert(cursorPos++, 1, static_cast<char>(event.text.unicode));
                        inputText2.setString(secondInput);
                    }
                }
                cursorVisible = true;
                cursorClock.restart();
            }

            // Обработка клавиш стрелок
            if (currentState == State::Input && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    if (cursorPos > 0) cursorPos--;
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    if ((activeInputField == 1 && cursorPos < firstInput.size()) || 
                        (activeInputField == 2 && cursorPos < secondInput.size())) {
                        cursorPos++;
                    }
                }
                cursorVisible = true;
                cursorClock.restart();
            }
        }

        // Мигание курсора
        if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        }

        // Отрисовка
        window.clear(sf::Color(40, 40, 40));
        window.draw(title);
        window.draw(backButton);
        window.draw(backButtonText);

        if (currentState == State::Input) {
            // Отрисовка полей ввода
            window.draw(inputField1);
            window.draw(inputField2);
            window.draw(inputLabel1);
            window.draw(inputLabel2);
            window.draw(inputText1);
            window.draw(inputText2);

            // Отрисовка курсора
            if (cursorVisible) {
                float cursorX = 55.f;
                if (activeInputField == 1) {
                    cursorX += font.getGlyph('A', 24, false).advance * cursorPos;
                    cursor.setPosition(cursorX, 105.f);
                } else {
                    cursorX += font.getGlyph('A', 24, false).advance * cursorPos;
                    cursor.setPosition(cursorX, 165.f);
                }
                window.draw(cursor);
            }

            // Отрисовка кнопки Compare
            compareButton.setFillColor(
                compareButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) ?
                sf::Color(0, 150, 255) : sf::Color(0, 120, 200));
            window.draw(compareButton);
            window.draw(compareButtonText);
        }
        else {
            // Адаптивная настройка столбцов
            const float windowWidth = window.getSize().x;
            float currentFontSize = initialFontSize;
            float columnWidth = 0.f;
            float totalWidth = 0.f;
            float startX = 0.f;

            // Функция для расчета максимальной ширины текста
            auto calculateMaxWidth = [&](const std::vector<std::string>& items) {
                float maxWidth = 0.f;
                sf::Text measureText("", font, currentFontSize);
                for (const auto& item : items) {
                    measureText.setString(item);
                    maxWidth = std::max(maxWidth, measureText.getLocalBounds().width);
                }
                return maxWidth;
            };

            // Подбираем оптимальный размер шрифта и ширину столбцов
            bool fits = false;
            while (!fits && currentFontSize >= 16.f) {
                float maxCommon = calculateMaxWidth(commonFeatures);
                float maxFirst = calculateMaxWidth(uniqueFirst);
                float maxSecond = calculateMaxWidth(uniqueSecond);

                columnWidth = std::max({maxCommon, maxFirst, maxSecond}) + 20.f;
                totalWidth = 3 * columnWidth + 2 * padding;
                startX = (windowWidth - totalWidth) / 2;

                if (totalWidth <= windowWidth - 2 * padding && startX >= padding) {
                    fits = true;
                } else {
                    currentFontSize -= 1.f;
                }
            }

            // Если все равно не помещается - выравниваем по левому краю
            if (!fits) {
                startX = padding;
                columnWidth = (windowWidth - 4 * padding) / 3;
            }

    // Отрисовка заголовков
    sf::Text header;
    header.setFont(font);
    header.setCharacterSize(currentFontSize + 2);
    header.setFillColor(sf::Color::Cyan);
    
    // Увеличиваем расстояние между колонками
    const float columnPadding = 30.f; // Было padding (20.f)
    
    header.setString("Common Features");
    header.setPosition(startX, startY - 40.f);
    window.draw(header);
    
    header.setString("Unique to " + firstInput);
    header.setPosition(startX + columnWidth + columnPadding, startY - 40.f);
    window.draw(header);
    
    header.setString("Unique to " + secondInput);
    header.setPosition(startX + 2 * (columnWidth + columnPadding), startY - 40.f);
    window.draw(header);

    // Отрисовка содержимого столбцов
    sf::Text itemText;
    itemText.setFont(font);
    itemText.setCharacterSize(currentFontSize);
    itemText.setFillColor(sf::Color::White);
    
    size_t maxRows = std::max({commonFeatures.size(), uniqueFirst.size(), uniqueSecond.size()});
    
    for (size_t i = 0; i < maxRows; ++i) {
        float yPos = startY + i * lineHeight;
        
        // Общие черты
        if (i < commonFeatures.size()) {
            itemText.setString(commonFeatures[i]);
            itemText.setPosition(startX, yPos);
            window.draw(itemText);
        }
        
        // Уникальные черты первого элемента
        if (i < uniqueFirst.size()) {
            itemText.setString(uniqueFirst[i]);
            itemText.setPosition(startX + columnWidth + columnPadding, yPos);
            window.draw(itemText);
        }
        
        // Уникальные черты второго элемента
        if (i < uniqueSecond.size()) {
            itemText.setString(uniqueSecond[i]);
            itemText.setPosition(startX + 2 * (columnWidth + columnPadding), yPos);
            window.draw(itemText);
        }
    }

        }

        window.display();
    }
}

Tree_errors akinator(Node_t* root)
{
    assert(root);

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
                compare_nodes(root);

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
    
    if (len > 0 && str[len-1] == '?')
    {
        str[len-1] = '\0';
    }

    return str;
};

Tree_errors compare_nodes(Node_t* root)
{
    assert(root);

    Node_t* first_node = nullptr;
    Node_t* second_node = nullptr;

    char first_elem[SIZE_ANSWER]  = {};
    char second_elem[SIZE_ANSWER] = {};

    verify_nodes(root, &first_node, &second_node, first_elem, second_elem);

    Path first_path  = {};
    Path second_path = {};

    build_path(root, first_node,  &first_path );
    build_path(root, second_node, &second_path);

    ON_DEBUG( (int i = 0; i < first_path.number_of_nodes; i++)
    {
        fprintf(stderr, YELLOW_TEXT("first_path: %s\n"), first_path.path[i]);
    }

    for (int i = 0; i < second_path.number_of_nodes; i++)
    {
        fprintf(stderr, YELLOW_TEXT("second_path: %s\n"), second_path.path[i]);
    })

    int common_part = 0;

    while (common_part < first_path.number_of_nodes &&
          common_part < second_path.number_of_nodes &&
          strcmp(first_path.path[common_part], second_path.path[common_part]) == 0)

          {
              common_part++;
          }

    output_common_features(common_part, first_path);

    output_unique_features(root, common_part, first_elem, first_path);
    output_unique_features(root, common_part, second_elem, second_path);

    for (int i = 0; i < first_path.number_of_nodes ; i++) free(first_path.path[i] );
    for (int i = 0; i < second_path.number_of_nodes; i++) free(second_path.path[i]);

    return SUCCESS;
}

Tree_errors verify_nodes(Node_t* root, Node_t** first_node, Node_t** second_node, char first_elem[], char second_elem[])
{

    do {
        fprintf(stderr, "enter first element: ");

        scanf("%s", first_elem);

        (*first_node) = search_node(root, first_elem);

        if ((*first_node) == nullptr) { fprintf(stderr, "Element '%s' not found. Please, try again.\n", first_elem); }

    } while ((*first_node) == nullptr);

    do {
        fprintf(stderr, "enter second element: ");

        scanf("%s", second_elem);

        (*second_node) = search_node(root, second_elem);

        if ((*second_node) == nullptr)  { fprintf(stderr, "Element '%s' not found. Please, try again.\n", second_elem); }

    } while ((*second_node) == nullptr);

    return SUCCESS;
}

Tree_errors output_unique_features(Node_t* root, int common_part, char first_elem[], Path path)
{
    assert(root);

    if (common_part < path.number_of_nodes)
    {
        fprintf(stderr, "unique to %s:\n", first_elem);

        for (int i = common_part; i < path.number_of_nodes; i++)
        {
            if (i > 0)
            {

                Node_t* parent = search_node(root, path.path[i - 1]);

              ON_DEBUG( if(parent && parent->right)
                    fprintf(stderr, PURPLE_TEXT("parent->data: %s parent->right->data: %s first_path.path[%d]: %s\n"), parent->data, parent->right->data, i, first_path.path[i]); )

                if (parent && parent->right && strcmp(parent->right->data, path.path[i]) == 0)
                {
                   ON_DEBUG(fprintf(stderr, GREEN_TEXT("parent->data: %s parent->right->data: %s first_path.path[%d]: %s\n"), parent->data, parent->right->data, i, first_path.path[i]); )

                    fprintf(stderr, "- No %s\n",remove_question_mark( path.path[i - 1]));
                }
                else
                {
                    fprintf(stderr, "- %s\n",remove_question_mark( path.path[i - 1]));
                }
            }
            
        }
    }

    return SUCCESS;
}

Tree_errors output_common_features(int common_part, Path path)
{
    
    fprintf(stderr, "common features: \n");

    if (common_part == 1)
    {
        fprintf(stderr, "nothing\n");
    }

    if (common_part > 0)
    {

        for (int i = 1; i < common_part; i++)
        {
            fprintf(stderr, "- %s\n",remove_question_mark( path.path[i]));
        }
    }

    return SUCCESS;
}
