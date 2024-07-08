#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

/**

\brief Hexagon represents a single field on the map and is built based on 6 points starting from the leftmost point, clockwise
\param isUsable There are 3 fields on the map that are not used in the game, but still need to be present. They have isUsable=false
\param isEmpty If a game piece is placed on the field, isEmpty=false
\param isPossibilityToMoveClose/Far This is important when selecting a move. In such cases, we want to display them differently to indicate where you can move
\param currentPlayer Indicates which player's game piece currently occupies the field
*/
struct Hexagon {
    bool isUsable;
    bool isEmpty;
    bool isPossibilityToMoveClose;
    bool isPossibilityToMoveFar;
    sf::Vector2f point1;
    sf::Vector2f point2;
    sf::Vector2f point3;
    sf::Vector2f point4;
    sf::Vector2f point5;
    sf::Vector2f point6;
    int currentPlayer;

    Hexagon(float x1, float y1, float x2, float y2, float x3, float y3,
            float x4, float y4, float x5, float y5, float x6, float y6,
            bool isUsable);

/**
 * \brief Provides two values as mouse.x and mouse.y and checks if they are inside the hexagon
 *
 * Divides the hexagon into 3 parts: left 2 triangles, a rectangle in the middle, and 2 triangles on the right.
 * Using the equations of the sides, it checks if the cursor is inside.
 * @param xPos sf::Mouse::getPosition(window).x
 * @param yPos sf::Mouse::getPosition(window).y
 * @return true if inside, false if not
 */
    auto isCursorIn(float xPos, float yPos) const -> bool;

/**
 * \brief Draws the Hexagon without any game pieces inside, representing the default state
 *
 * @param window sf::RenderWindow on which we want to draw it
 */
    auto drawHexagon(sf::RenderWindow &window) -> void;

/**
 * \brief Draws the Hexagon along with a circle in the center representing occupation by a specific player. The circle has a different color depending on the player number
 *
 * @param window sf::RenderWindow on which we want to draw it
 * @param CurrentPlayer the player number to which the field currently belongs
 */
    auto drawHexagonTaken(sf::RenderWindow &window, int CurrentPlayer) -> void;

/**
 * \brief Draws a slightly more highlighted Hexagon, which shows the nearest Hexagons around the one chosen by the player
 * (since we distinguish behavior based on the distance of the Hexagon from the selection, we also want to distinguish its appearance from the more distant ones)
 * @param window sf::RenderWindow on which we want to draw it
 */
    auto drawMovePossibilityClose(sf::RenderWindow &window) -> void;

/**
 * \brief Draws a slightly less highlighted Hexagon to show the possibilities of a further move that jumps instead of copies to that field
 *
 * @param window sf::RenderWindow on which we want to draw it
 */
    auto drawMovePossibilityFar(sf::RenderWindow &window) -> void;

};