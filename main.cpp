#include <iostream>
#include <cmath>
#include <sstream>
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "GamePanel.h"
#include <filesystem>
#include <fstream>
#include <thread>


int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Hexagon",sf::Style::Titlebar | sf::Style::Close);
    GamePanel gamePanel = GamePanel();
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonReleased) {
                int mouseXPosition=sf::Mouse::getPosition(window).x;
                int mouseYPosition=sf::Mouse::getPosition(window).y;
                if (gamePanel.startPanelOn) {
                    gamePanel.manageStartPanel(mouseXPosition,mouseYPosition);
                } else if (gamePanel.bestGamesScreenOn) {
                   gamePanel.manageBestGamesPanel(mouseXPosition,mouseYPosition);
                } else if (gamePanel.gameChoiceScreenOn) {
                    gamePanel.manageGameModeChoicePanel(mouseXPosition,mouseYPosition);
                }else if(gamePanel.gamePanelOn) {
                   gamePanel.manageGamePanel(mouseXPosition,mouseYPosition,window);
                }else if(gamePanel.endOfGameScreenOn){
                    gamePanel.manageFinishPanel(mouseXPosition,mouseYPosition);
                }
            }
        }
        if (gamePanel.checkIfOver()) {
            gamePanel.gamePanelOn=false;
            gamePanel.endOfGameScreenOn = true;
        } else {
            // if we're playing against AI thats where it does its move
            if (gamePanel.mode == vsAI && gamePanel.currentPlayerMove == 2 && !gamePanel.isMoving) {
                gamePanel.aiMove();
            }
        }
        window.clear(sf::Color(50, 16, 81));
        gamePanel.draw(window);
        window.display();
    }
    gamePanel.manageFiles();
    return 0;
}

