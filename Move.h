#pragma once


#include "SFML/System/Vector2.hpp"
/**
 * \brief represents a move between locations from (x1,y1) to (x2,y2) (here as start and end) isFar helps to determine what type of move user wants to make
 * amountTaken will be used to determine the best move for AI
 */
struct Move{
    sf::Vector2i start;
    sf::Vector2i end;
    bool isFar;
    int amountTaken=0;
    Move(sf::Vector2i start, sf::Vector2i end, bool isFar);
};