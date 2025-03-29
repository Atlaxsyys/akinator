#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstring>

#include "tree.h"

void renderAkinator(sf::RenderWindow &window, Node_t* &currentNode, sf::Font &font);
void renderMenu(sf::RenderWindow &window, sf::Font &font, int &selectedOption);
void show_data_base();
void renderCompareNodes(sf::RenderWindow& window, Node_t* root, sf::Font& font);