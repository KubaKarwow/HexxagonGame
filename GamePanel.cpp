#include <vector>
#include <map>
#include "GamePanel.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include <math.h>
#include <iostream>
#include "Hexagon.h"
#include "SFML/Graphics/Sprite.hpp"
#include <fstream>
#include <sstream>
#include <thread>
#include <filesystem>


GamePanel::GamePanel() {
    gameChoiceScreenOn = false;


    bestGamesScreenOn = false;
    wrongInput = false;
    endOfGameScreenOn = false;
    gamePanelOn = false;
    startPanelOn = true;
    choosingMove = false;
    fun = false;
    isMoving = false;
    gameMap.push_back(1);
    gameMap.push_back(2);
    gameMap.push_back(3);
    gameMap.push_back(4);
    gameMap.push_back(5);
    gameMap.push_back(4);
    gameMap.push_back(5);
    gameMap.push_back(4);
    gameMap.push_back(5);
    gameMap.push_back(4);
    gameMap.push_back(5);
    gameMap.push_back(4);
    gameMap.push_back(5);
    gameMap.push_back(4);
    gameMap.push_back(3);
    gameMap.push_back(2);
    gameMap.push_back(1);
    radius = 50.f;
    sideLength = radius * sqrt(3);
    scoreOne = 3;
    scoreTwo = 3;
    currentPlayerMove = 1;
    sf::Vector2f topLeft(500 - radius, 100);
    for (int row = 0; row < gameMap.size(); row++) {
        auto line = std::vector<Hexagon>();
        float toRevisit = topLeft.x;
        for (int col = 0; col < gameMap[row]; col++) {
            if (!((row == 6 && col == 2) || (row == 9 && col == 1) || (row == 9 && col == 2))) {
                auto hex = Hexagon(topLeft.x, topLeft.y + sideLength / 2,
                                   topLeft.x + radius / 2.f, topLeft.y,
                                   topLeft.x + 1.5f * radius, topLeft.y,
                                   topLeft.x + 2 * radius, topLeft.y + sideLength / 2,
                                   topLeft.x + 1.5f * radius, topLeft.y + sideLength,
                                   topLeft.x + 0.5f * radius, topLeft.y + sideLength, true);
                line.push_back(hex);
                if (row == 0 || (row == 12 && col == 0) || (row == 12 && col == 4)) {
                    line[col].currentPlayer = 2;
                    line[col].isEmpty = false;
                } else if (row == 4 && (col == 0 || col == 4) || row == 16) {
                    line[col].currentPlayer = 1;
                    line[col].isEmpty = false;
                }


            } else {
                auto hex = Hexagon(topLeft.x, topLeft.y + sideLength / 2,
                                   topLeft.x + radius / 2.f, topLeft.y,
                                   topLeft.x + 1.5f * radius, topLeft.y,
                                   topLeft.x + 2 * radius, topLeft.y + sideLength / 2,
                                   topLeft.x + 1.5f * radius, topLeft.y + sideLength,
                                   topLeft.x + 0.5f * radius, topLeft.y + sideLength, false);
                line.push_back(hex);
            }
            if (col != gameMap[row] - 1) {
                topLeft.x += 3 * radius;
            }
        }
        topLeft.y += sideLength / 2.f;
        topLeft.x = toRevisit;
        if (row == 4 || row == 6 || row == 8 || row == 10 || row >= 12) {
            topLeft.x += radius * 1.5f;
        } else {
            topLeft.x -= radius * 1.5f;
        }
        hexagons.push_back(line);
    }

    font.loadFromFile("..\\Resources\\Gameplay.ttf");
    vsPlayerBox.setSize(sf::Vector2f(400.0f, 100));
    vsPlayerBox.setFillColor(sf::Color(230, 230, 250));
    vsPlayerBox.setPosition(300, 450);
    vsAIBox.setSize(sf::Vector2f(400.0f, 100));
    vsAIBox.setFillColor(sf::Color(216, 191, 216));
    vsAIBox.setPosition(300, 600);
    newGameBox.setSize(sf::Vector2f(400.0f, 100));
    newGameBox.setFillColor(sf::Color(230, 230, 250));
    newGameBox.setPosition(300, 250);
    loadGameBox.setSize(sf::Vector2f(400.0f, 100));
    loadGameBox.setFillColor(sf::Color(216, 191, 216));
    loadGameBox.setPosition(300, 400);
    bestGamesBox.setSize(sf::Vector2f(400.0f, 100));
    bestGamesBox.setFillColor(sf::Color(186, 211, 216));
    bestGamesBox.setPosition(300, 550);
    pawnMoving.setRadius(35.f);
    pawnMoving.setOrigin(35, 35);
    pawnMoving.setPosition(-100, -100);

}

auto GamePanel::showPossibilities(int player, int row, int col,
                                  sf::RenderWindow &window) -> std::vector<Move> {
    auto moves = std::vector<Move>();
    for (int i = 0; i < hexagons.size(); i++) {
        for (int j = 0; j < hexagons[i].size(); j++) {
            // these conditions mean that other hexagons which we want to consider can only be that far away
            if (i >= row - 4 && i <= row + 4) {
                if (j >= col - 2 && j <= col + 2) {
                    if (hexagons[i][j].isEmpty && hexagons[i][j].isUsable && !(row == i && col == j)) {
                        float howFar = pow(((hexagons[i][j].point1.x + 50)
                                            - (hexagons[row][col].point1.x + 50)), 2)
                                       + pow((hexagons[i][j].point1.y
                                              - hexagons[row][col].point1.y), 2);
                        if (sqrt(howFar) <= 50 * sqrt(3)) {
                            moves.push_back(Move(sf::Vector2i(row, col), sf::Vector2i(i, j), false));
                            hexagons[i][j].isPossibilityToMoveClose = true;

                        } else if (sqrt(howFar) >= 150 && sqrt(howFar) <= (sqrt(3) * 100)) { //2h albo 3r
                            moves.push_back(Move(sf::Vector2i(row, col), sf::Vector2i(i, j), true));
                            hexagons[row][col].isPossibilityToMoveClose = true;
                        }
                    }
                }
            }
        }
    }
    return moves;
}

auto GamePanel::showAmountofPossibilities(int row, int col) const -> int {
    int result = 0;
    for (int i = 0; i < hexagons.size(); i++) {
        for (int j = 0; j < hexagons[i].size(); j++) {
            if (i >= row - 4 && i <= row + 4) {
                if (j >= col - 2 && j <= col + 2) {
                    if (hexagons[i][j].isEmpty && hexagons[i][j].isUsable && !(row == i && col == j)) {
                        float howFar = pow(((hexagons[i][j].point1.x + 50) - (hexagons[row][col].point1.x + 50)), 2)
                                       + pow((hexagons[i][j].point1.y - hexagons[row][col].point1.y), 2);
                        if (sqrt(howFar) <= 50 * sqrt(3) + 15) {
                            result++;
                        } else if (sqrt(howFar) >= 150 && sqrt(howFar) <= (sqrt(3) * 100)) { //2h albo 3r +/- 20
                            result++;
                        }
                    }
                }
            }
        }
    }
    return result;
}

auto GamePanel::convertAround(int player, int row, int col) -> void {
    for (int i = 0; i < hexagons.size(); i++) {
        for (int j = 0; j < hexagons[i].size(); j++) {
            if ((i >= row - 2 && i <= row + 2) & (j >= col - 1 && j <= col + 1)) {
                if (!hexagons[i][j].isEmpty && hexagons[i][j].currentPlayer != hexagons[row][col].currentPlayer) {
                    float howFar = pow(((hexagons[i][j].point1.x + 50)
                                        - (hexagons[row][col].point1.x + 50)), 2)
                                   + pow((hexagons[i][j].point1.y
                                          - hexagons[row][col].point1.y), 2);
                    if (sqrt(howFar) <= 50 * sqrt(3)) {
                        hexagons[i][j].currentPlayer = hexagons[row][col].currentPlayer;
                        if (hexagons[row][col].currentPlayer == 1) {
                            scoreOne++;
                            scoreTwo--;
                        } else if (hexagons[row][col].currentPlayer == 2) {
                            scoreTwo++;
                            scoreOne--;
                        }
                    }
                }
            }
        }
    }
}

auto GamePanel::draw(sf::RenderWindow &window) -> void {
    if (endOfGameScreenOn) drawFinishPanel(window);
    else if (startPanelOn) drawStartPanel(window);
    else if (gameChoiceScreenOn) drawChoicePanel(window);
    else if (bestGamesScreenOn) drawBestGamesPanel(window);
    else drawGamePanel(window);
}

auto GamePanel::moveClose(int player, int startRow, int startCol, int finishRow, int finishCol) -> void {
    if (player == 1) {
        scoreOne++;
        pawnMoving.setFillColor(sf::Color::Red);
    } else {
        pawnMoving.setFillColor(sf::Color::Blue);
        scoreTwo++;
    }
    pawnMoving.setPosition(hexagons[startRow][startCol].point1.x + 50, hexagons[startRow][startCol].point1.y);
    end = sf::Vector2f(finishRow, finishCol);
    counterForMove = 0;
    isMoving = true;
}

auto GamePanel::moveFar(int player, int startRow, int startCol, int finishRow, int finishCol) -> void {

    hexagons[startRow][startCol].isEmpty = true;
    hexagons[startRow][startCol].currentPlayer = 0;
    hexagons[startRow][startCol].isPossibilityToMoveClose = false;
    hexagons[startRow][startCol].isPossibilityToMoveFar = false;
    if (player == 1) pawnMoving.setFillColor(sf::Color::Red);
    else pawnMoving.setFillColor(sf::Color::Blue);
    pawnMoving.setPosition(hexagons[startRow][startCol].point1.x + 50, hexagons[startRow][startCol].point1.y);
    end = sf::Vector2f(finishRow, finishCol);
    counterForMove = 0;
    isMoving = true;
}

auto GamePanel::drawGamePanel(sf::RenderWindow &window) -> void {
    score1.setFont(font);
    score1.setString("p1: " + std::to_string(scoreOne));
    score1.setCharacterSize(50);
    score1.setFillColor(sf::Color::Red);
    score1.setPosition(15, 15);
    score2.setFont(font);
    score2.setString("p2: " + std::to_string(scoreTwo));
    score2.setCharacterSize(50);
    score2.setFillColor(sf::Color::Blue);
    score2.setPosition(985 - score2.getGlobalBounds().width, 15);
    currentMove.setFont(font);
    currentMove.setString("Current Move: P" + std::to_string(currentPlayerMove));
    currentMove.setCharacterSize(50);
    currentMove.setFillColor(sf::Color::Magenta);
    currentMove.setPosition(500 - currentMove.getGlobalBounds().width / 2.f, 15);
    window.draw(score1);
    window.draw(score2);
    window.draw(currentMove);

    if (isMoving) {
        if (counterForMove <= 60) {
            pawnMoving.move(sf::Vector2f((hexagons[end.x][end.y].point1.x + 50 - pawnMoving.getPosition().x) / 20.f,
                                         (hexagons[end.x][end.y].point1.y
                                          - pawnMoving.getPosition().y) / 20.f));
            counterForMove++;
        } else {
            pawnMoving.move(sf::Vector2f(hexagons[end.x][end.y].point1.x + 50 - pawnMoving.getPosition().x,
                                         hexagons[end.x][end.y].point1.y - pawnMoving.getPosition().y));
            int previousPlayerMove = 0;
            if (currentPlayerMove == 2) previousPlayerMove = 1;
            else previousPlayerMove = 2;
            hexagons[end.x][end.y].isEmpty = false;
            hexagons[end.x][end.y].currentPlayer = previousPlayerMove;
            convertAround(previousPlayerMove, end.x, end.y);
            isMoving = false;
        }
    }

    // the actual important loop for drawing
    for (int i = 0; i < hexagons.size(); i++) {
        for (int j = 0; j < hexagons[i].size(); j++) {
            if (hexagons[i][j].isUsable) {
                if (hexagons[i][j].isEmpty) {
                    if (hexagons[i][j].isPossibilityToMoveClose) {
                        hexagons[i][j].drawMovePossibilityClose(window);
                    } else if (hexagons[i][j].isPossibilityToMoveFar) {
                        hexagons[i][j].drawMovePossibilityFar(window);
                    } else {
                        hexagons[i][j].drawHexagon(window);
                    }
                } else {
                    hexagons[i][j].drawHexagonTaken(window, hexagons[i][j].currentPlayer);
                }
            }
        }
    }
    window.draw(pawnMoving);
}

auto GamePanel::drawChoicePanel(sf::RenderWindow &window) const -> void {
    sf::Text textOpener;
    sf::Text textOption1;
    sf::Text textOption2;
    sf::Text textInfo;

    sf::RectangleShape rectangleShape(sf::Vector2f(400.0f, 100));
    sf::RectangleShape rectangleShape2(sf::Vector2f(400, 100));
    rectangleShape.setFillColor(sf::Color(230, 230, 250));
    rectangleShape.setPosition(300, 450);
    rectangleShape2.setFillColor(sf::Color(216, 191, 216));
    rectangleShape2.setPosition(300, 600);

    textOpener.setFont(font);
    textOpener.setString("Hello there! :D");
    textOpener.setCharacterSize(50);
    textOpener.setFillColor(sf::Color::Magenta);
    textOpener.setPosition(280, 50);

    textInfo.setFont(font);
    textInfo.setString("Please select your game mode");
    textInfo.setCharacterSize(50);
    textInfo.setFillColor(sf::Color::Magenta);
    textInfo.setPosition(20, 260);

    textOption1.setFont(font);
    textOption1.setString("vs Player");
    textOption1.setCharacterSize(50);
    textOption1.setFillColor(sf::Color::Blue);
    textOption1.setPosition(330, 470);

    textOption2.setFont(font);
    textOption2.setString("vs AI :O");
    textOption2.setCharacterSize(50);
    textOption2.setFillColor(sf::Color::Red);
    textOption2.setPosition(380, 620);

    window.draw(vsPlayerBox);
    window.draw(textOpener);
    window.draw(textInfo);
    window.draw(vsAIBox);
    window.draw(textOption1);
    window.draw(textOption2);
}

auto GamePanel::drawStartPanel(sf::RenderWindow &window) const -> void {


    sf::Text helloThere;
    sf::Text newGame;
    sf::Text loadGame;
    sf::Text bestGames;

    helloThere.setFont(font);
    helloThere.setString("Hello there! :D");
    helloThere.setCharacterSize(50);
    helloThere.setFillColor(sf::Color::Magenta);
    helloThere.setPosition(280, 50);

    newGame.setFont(font);
    newGame.setString("New Game");
    newGame.setCharacterSize(50);
    newGame.setFillColor(sf::Color::Magenta);
    newGame.setPosition(500 - newGame.getGlobalBounds().width / 2, 300 - newGame.getGlobalBounds().height / 2);

    loadGame.setFont(font);
    loadGame.setString("Load Game");
    loadGame.setCharacterSize(50);
    loadGame.setFillColor(sf::Color::Blue);
    loadGame.setPosition(500 - loadGame.getGlobalBounds().width / 2, 450 - loadGame.getGlobalBounds().height / 2);

    bestGames.setFont(font);
    bestGames.setString("Best Games");
    bestGames.setCharacterSize(50);
    bestGames.setFillColor(sf::Color::Red);
    bestGames.setPosition(500 - bestGames.getGlobalBounds().width / 2, 600 - loadGame.getGlobalBounds().height / 2);

    window.draw(newGameBox);
    window.draw(loadGameBox);
    window.draw(bestGamesBox);
    window.draw(helloThere);
    window.draw(newGame);
    window.draw(loadGame);
    window.draw(bestGames);

}

auto GamePanel::drawFinishPanel(sf::RenderWindow &window) -> void {
    sf::Text textOver;
    sf::Text textWinner;
    sf::Text textscore1;
    sf::Text textscore2;
    sf::Text clickme;


    bonusButton.setFillColor(sf::Color::Yellow);
    bonusButton.setSize(sf::Vector2f(100, 100));
    bonusButton.setPosition(sf::Vector2f(900, 900));

    sf::RectangleShape rectangleShape(sf::Vector2f(500.0f, 500));
    rectangleShape.setFillColor(sf::Color(32, 178, 170));
    rectangleShape.setPosition(250, 250);

    textOver.setFont(font);
    textOver.setString("GAME OVER! ");
    textOver.setCharacterSize(70);
    textOver.setFillColor(sf::Color::Red);
    textOver.setPosition(265, 280);

    clickme.setFont(font);
    clickme.setString("click me plox ");
    clickme.setCharacterSize(50);
    clickme.setFillColor(sf::Color::Yellow);
    clickme.setPosition(900 - clickme.getGlobalBounds().width, 950);

    if (scoreTwo > scoreOne) {
        textWinner.setString("Player2 wins!");
    } else if (scoreOne > scoreTwo) {
        textWinner.setString("Player1 wins!");
    } else {
        textWinner.setString("Draw!");
    }
    textWinner.setFont(font);
    textWinner.setCharacterSize(50);
    textWinner.setFillColor(sf::Color::Magenta);
    textWinner.setPosition(500 - textWinner.getGlobalBounds().width / 2, 500);

    score1.setString("p1: " + std::to_string(scoreOne));
    score1.setCharacterSize(30);
    score1.setPosition(260, 740 - score1.getGlobalBounds().height / 2 - 10);
    score1.setFillColor(sf::Color::Red);


    score2.setString("p2: " + std::to_string(scoreTwo));
    score2.setCharacterSize(30);
    score2.setPosition(740 - score2.getGlobalBounds().width, 740 - score2.getGlobalBounds().height / 2 - 10);
    score2.setFillColor(sf::Color::Blue);

    sf::Texture texture;
    texture.loadFromFile("..\\Resources\\meme.png");
    sf::Sprite sprite(texture);
    sprite.setPosition(500 - sprite.getGlobalBounds().width / 2, 500 - sprite.getGlobalBounds().height / 2);
    if (fun) {
        window.draw(sprite);
    } else {
        window.draw(rectangleShape);
        window.draw(textOver);
        window.draw(textWinner);
        window.draw(score1);
        window.draw(score2);
        window.draw(bonusButton);
        window.draw(clickme);
    }

}

auto GamePanel::checkIfOver() -> bool {
    if (isMoving) return false;
    int player1Possibilities = 0;
    int player2Possibilities = 0;
    if (scoreOne == 0 || scoreTwo == 0 || scoreOne + scoreTwo == 58) {
        return true;
    }
    for (int i = 0; i < hexagons.size(); i++) {
        for (int j = 0; j < hexagons[i].size(); j++) {
            if (hexagons[i][j].currentPlayer == 1) {
                if (showAmountofPossibilities(i, j) == 0) {
                    player1Possibilities++;
                }
            } else {
                if (showAmountofPossibilities(i, j) == 0) {
                    player2Possibilities++;
                }
            }
        }
    }
    return player1Possibilities == scoreOne || player2Possibilities == scoreTwo;

}

auto GamePanel::bestGames(const std::string &gName) const -> void {
    auto file = std::fstream("bestGames.txt", std::ios::in);
    auto vec = std::vector<std::string>();
    int lineIt = 0;
    for (std::string line; std::getline(file, line); lineIt++) {
        std::string word;
        int wordIt = 0;
        int playersDiff = 0;
        std::string name;
        auto stringstream = std::stringstream(line);
        for (word; stringstream >> word; wordIt++) {
            if (wordIt == 0) name = word;
            if (wordIt == 1) playersDiff = std::stoi(word);
            if (wordIt == 2) playersDiff = abs(playersDiff - std::stoi(word));
            vec.push_back(word);
        }
    }
    file.close();
    // here we're checking if there are 5 lines and if so is the current game better than any of them
    int thisGame = abs(scoreOne - scoreTwo);
    int min = thisGame;
    int indexmina = -1;
    if (vec.size() < 13) {
        auto fileSave = std::fstream("bestGames.txt", std::ios::app);
        fileSave << gName << " " << std::to_string(scoreOne) << " " << std::to_string(scoreTwo) << '\n';
        fileSave.close();
    }// there are atleast 5 lines
    else {
        auto fileRedo = std::fstream("bestGames.txt", std::ios::out | std::ios::trunc);
        for (int i = 0; i < vec.size(); i += 3) {
            int previousGame = abs(std::stoi(vec[i + 1]) - std::stoi(vec[i + 2]));
            if (min > previousGame) {
                min = previousGame;
                indexmina = i;
            }
        }
        if (indexmina > -1) {
            vec[indexmina] = gName;
            vec[indexmina + 1] = std::to_string(scoreOne);
            vec[indexmina + 2] = std::to_string(scoreTwo);
        }
        for (int i = 0; i < vec.size(); i += 3) {
            fileRedo << vec[i] << " " << vec[i + 1] << " " << vec[i + 2] << '\n';
        }
        fileRedo.close();

    }

}

auto GamePanel::saveToFile(const std::string &fileName) -> void {
    auto saving = std::fstream(fileName, std::ios::out | std::ios::trunc);
    saving << currentPlayerMove << '\n';
    saving << endOfGameScreenOn << '\n';
    saving << mode << '\n';
    saving << scoreOne << '\n';
    saving << scoreTwo << '\n';
    for (int i = 0; i < hexagons.size(); i++) {
        for (int j = 0; j < hexagons[i].size(); j++) {
            saving << i << ' ' << j << ' ' << hexagons[i][j].isUsable << ' '
                   << hexagons[i][j].isEmpty << ' ' << hexagons[i][j].currentPlayer << '\n';
        }
    }
    saving.close();
}

auto GamePanel::readFromFile(const std::string &fileName) -> void {

    auto file = std::fstream(fileName);
    int iterator = 0;
    for (std::string line; std::getline(file, line);) {
        std::string word;
        if (iterator == 0) {
            currentPlayerMove = std::stoi(line);
        } else if (iterator == 1) {
            endOfGameScreenOn = std::stoi(line);
        } else if (iterator == 2) {
            if (std::stoi(line) == 0) {
                mode = vsPlayer;
            } else {
                mode = vsAI;
            }
        } else if (iterator == 3) {
            scoreOne = std::stoi(line);
            score1.setString(line);
        } else if (iterator == 4) {
            scoreTwo = std::stoi(line);
            score2.setString(line);
        } else {
            auto stringstream = std::stringstream(line);
            auto info = std::vector<int>();
            for (word; stringstream >> word;) {
                info.push_back(std::stoi(word));
            }
            hexagons[info[0]][info[1]].isUsable = info[2];
            hexagons[info[0]][info[1]].isEmpty = info[3];
            hexagons[info[0]][info[1]].currentPlayer = info[4];
        }
        iterator++;
    }
    file.close();
}

auto GamePanel::drawBestGamesPanel(sf::RenderWindow &window) -> void {
    sf::Text hello;
    sf::Text bscore1;
    sf::Text goBack;

    auto file = std::fstream("bestGames.txt", std::ios::in);
    auto lines = std::vector<std::string>();
    for (std::string line; std::getline(file, line);) {
        lines.push_back(line);
    }
    file.close();
    sf::RectangleShape box(sf::Vector2f(600, 700));
    box.setPosition(200, 100);
    box.setFillColor(sf::Color(147, 118, 224));

    backButton.setSize(sf::Vector2f(100, 100));
    backButton.setFillColor(sf::Color(191, 172, 226));
    backButton.setPosition(450, 850);

    hello.setFont(font);
    hello.setString("5 BEST GAMES");
    hello.setCharacterSize(50);
    hello.setFillColor(sf::Color::Green);
    hello.setPosition(500 - hello.getGlobalBounds().width / 2, 150);
    window.draw(box);
    for (int i = 0; i < lines.size(); i++) {
        bscore1.setFont(font);
        bscore1.setString(lines[i]);
        bscore1.setCharacterSize(50);
        bscore1.setFillColor(sf::Color::Green);
        bscore1.setPosition(500 - bscore1.getGlobalBounds().width / 2, 250 + i * 100);
        window.draw(bscore1);
    }
    goBack.setFont(font);
    goBack.setString("B");
    goBack.setCharacterSize(50);
    goBack.setFillColor(sf::Color::Red);
    goBack.setPosition(500 - goBack.getGlobalBounds().width / 2, 900 - goBack.getGlobalBounds().height / 2);

    window.draw(hello);
    window.draw(backButton);
    window.draw(goBack);

}

auto GamePanel::currentBestMove() const -> Move {

    auto moves = std::vector<Move>();
    for (int row = 0; row < hexagons.size(); row++) {
        for (int col = 0; col < hexagons[row].size(); col++) {
            if (hexagons[row][col].currentPlayer == currentPlayerMove) {
                // we're counting every move for every pawn
                for (int i = 0; i < hexagons.size(); i++) {
                    for (int j = 0; j < hexagons[i].size(); j++) {
                        if (i >= row - 4 && i <= row + 4) {
                            if (j >= col - 2 && j <= col + 2) {
                                if (hexagons[i][j].isEmpty && hexagons[i][j].isUsable && !(row == i && col == j)) {
                                    float howFar = pow(((hexagons[i][j].point1.x + 50)
                                                        - (hexagons[row][col].point1.x + 50)), 2)
                                                   + pow((hexagons[i][j].point1.y
                                                          - hexagons[row][col].point1.y), 2);
                                    if (sqrt(howFar) <= 50 * sqrt(3)) {
                                        moves.push_back(Move(sf::Vector2i(row, col), sf::Vector2i(i, j), false));
                                    } else if (sqrt(howFar) >= 150 &&
                                               sqrt(howFar) <= (sqrt(3) * 100)) { //2h albo 3r +/- 20
                                        moves.push_back(Move(sf::Vector2i(row, col), sf::Vector2i(i, j), true));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // here we're counting the amount of pawns taken by a move
    for (int movesI = 0; movesI < moves.size(); movesI++) {
        for (int i = 0; i < hexagons.size(); i++) {
            for (int j = 0; j < hexagons[i].size(); j++) {
                if ((i >= moves[movesI].end.x - 2 && i <= moves[movesI].end.x + 2) &
                    (j >= moves[movesI].end.y - 1 && j <= moves[movesI].end.y + 1)) {
                    if (!hexagons[i][j].isEmpty && hexagons[i][j].currentPlayer !=
                                                   hexagons[moves[movesI].start.x][moves[movesI].start.y].currentPlayer) {
                        float howFar = pow(((hexagons[i][j].point1.x + 50)
                                            - (hexagons[moves[movesI].end.x][moves[movesI].end.y].point1.x + 50)), 2)
                                       + pow((hexagons[i][j].point1.y
                                              - hexagons[moves[movesI].end.x][moves[movesI].end.y].point1.y), 2);
                        if (sqrt(howFar) <= 50 * sqrt(3)) {
                            moves[movesI].amountTaken++;

                        }
                    }
                }
            }
        }
    }
    // now we're just checking which moves takes over the most pawns and return it
    int max = 0;
    int indexmaxa = 0;
    for (int i = 0; i < moves.size(); i++) {
        if (moves[i].amountTaken > max) {
            max = moves[i].amountTaken;
            indexmaxa = i;
        }
    }
    return moves[indexmaxa];
}

auto GamePanel::aiMove() -> void {
    Move move = currentBestMove();
    if (move.isFar) moveFar(currentPlayerMove, move.start.x, move.start.y, move.end.x, move.end.y);
    else moveClose(currentPlayerMove, move.start.x, move.start.y, move.end.x, move.end.y);
    if (currentPlayerMove == 1) currentPlayerMove = 2;
    else currentPlayerMove = 1;
}

auto GamePanel::manageStartPanel(int mouseXPosition, int mouseYPosition) -> void {
    std::string fullGameName;
    bool oldGamesExist = false;
    auto makingOldGames = std::filesystem::directory_iterator(".");
    for (const auto &entry: makingOldGames) {
        if (entry.path().filename() == "Old_Games") oldGamesExist = true;
    }
    if (!oldGamesExist) std::filesystem::create_directory("Old_Games");
    if (newGameBox.getGlobalBounds().contains(mouseXPosition, mouseYPosition)) {
        // handling making a new game
        std::cout << "insert your gameName please!\n";
        std::cin >> gameName;
        fullGameName = "Old_Games\\" + gameName;
        auto dirIt = std::filesystem::directory_iterator("Old_Games");
        for (auto const &entry: dirIt) {
            if (entry.path().string() == gameName) {
                std::cout << "file like that already exists!\n";
                wrongInput = true;
            }
        }
        if (!wrongInput) {
            startPanelOn = false;
            gameChoiceScreenOn = true;
        }
    } else if (loadGameBox.getGlobalBounds().contains(mouseXPosition, mouseYPosition)) {
        // handling loading a game from a file
        bool oldGamesExist = false;
        auto makingOldGames = std::filesystem::directory_iterator(".");
        for (const auto &entry: makingOldGames) {
            if (entry.path().filename() == "Old_Games") oldGamesExist = true;
        }
        if (!oldGamesExist) {
            std::filesystem::create_directory("Old_Games");
            std::cout << "There are no previous games to be loaded :C\n";
        } else {
            auto checkingAmountOfFiles = std::filesystem::directory_iterator("Old_Games");
            int amountOffiles = 0;
            for (auto const &entry: checkingAmountOfFiles) {
                if (entry.is_regular_file()) amountOffiles++;
            }
            if (amountOffiles == 0) {
                std::cout << "There are no previous games to be loaded :C\n";
            } else {
                std::cout << "here is a list of game files\n";
                auto oldGamesIterator = std::filesystem::directory_iterator("Old_Games");
                for (const auto &entry: oldGamesIterator) {
                    std::cout << entry.path().filename().string() << '\n';
                }
                std::cout << "please insert the name of the desired game to load\n";
                std::cin >> gameName;
                wrongInput = true;
                oldGamesIterator = std::filesystem::directory_iterator("Old_Games");
                for (const auto &entry: oldGamesIterator) {
                    if (entry.path().filename().string() == gameName) {
                        wrongInput = false;
                    }
                }
                if (!wrongInput) {
                    gameChoiceScreenOn = false;
                    startPanelOn = false;
                    gamePanelOn = true;
                    fullGameName = "Old_Games\\" + gameName;
                    readFromFile(fullGameName);
                } else {
                    std::cout << "File that like that doesn't exist\n";
                }
            }
        }
    } else if (bestGamesBox.getGlobalBounds().contains(mouseXPosition, mouseYPosition)) {
        bestGamesScreenOn = true;
        startPanelOn = false;
    }
}

auto GamePanel::manageBestGamesPanel(int mouseXPosition, int mouseYPosition) -> void {
    if (backButton.getGlobalBounds().contains(mouseXPosition, mouseYPosition)) {
        startPanelOn = true;
        bestGamesScreenOn = false;
    }
}

auto GamePanel::manageGameModeChoicePanel(int mouseXPosition, int mouseYPosition) -> void {
    if (vsPlayerBox.getGlobalBounds().contains(mouseXPosition, mouseYPosition)) {
        // choosing to play against a different player
        gameChoiceScreenOn = false;
        gamePanelOn = true;
        choosingMove = false;
        mode = vsPlayer;
    } else if (vsAIBox.getGlobalBounds().contains(mouseXPosition, mouseYPosition)) {
        // selecting playing vs AI
        gameChoiceScreenOn = false;
        gamePanelOn = true;
        choosingMove = false;
        mode = vsAI;
    }
}

auto GamePanel::focusedHexagon(int mouseXpos, int mouseYpos) const -> sf::Vector2i {
    int yPos = (mouseYpos - 100) / (50 * sqrt(3) / 2.0);
    if (yPos == 17) {
        return sf::Vector2i(16, 0);
    }
    int mostleftX = 0;
    if (yPos <= 4) {
        mostleftX = 450 - 75 * yPos;
    } else if (yPos < 12) {
        if (yPos % 2 != 0) mostleftX = 225;
        else mostleftX = 150;
    } else {
        mostleftX = 150 + 75 * (yPos - 12);
    }

    int xPosToDivide = (mouseXpos - mostleftX) / 75;
    int xPos = 0;
    if (xPosToDivide % 2 == 0) {
        xPos = xPosToDivide / 2;
    } else {
        xPos = (xPosToDivide - 1) / 2;

    }
    if(xPos==1 && yPos==16) return {15,1};
    double a1 = (hexagons[yPos][xPos].point1.y - hexagons[yPos][xPos].point2.y) /
                (hexagons[yPos][xPos].point1.x - hexagons[yPos][xPos].point2.x);
    double a2 = (hexagons[yPos][xPos].point3.y - hexagons[yPos][xPos].point4.y) /
                (hexagons[yPos][xPos].point3.x - hexagons[yPos][xPos].point4.x);
    double b1 = hexagons[yPos][xPos].point1.y - hexagons[yPos][xPos].point1.x * a1;
    double b2 = hexagons[yPos][xPos].point3.y - hexagons[yPos][xPos].point3.x * a2;
    if (mouseXpos >= hexagons[yPos][xPos].point3.x && mouseXpos <= hexagons[yPos][xPos].point4.x) {
        if (mouseYpos <= a2 * mouseXpos + b2) {
            if (yPos > 4 && yPos < 13 && yPos % 2 != 0) {
                yPos--;
                xPos++;
            } else if (yPos >= 13) {
                yPos--;
                xPos++;
            } else {
                yPos--;
            }
        }
    } else if (mouseXpos > hexagons[yPos][xPos].point4.x) {
        if (yPos > 4 && yPos < 13 && yPos % 2 != 0) {
            yPos--;
            xPos++;
        } else if (yPos >= 13) {
            yPos--;
            xPos++;
        } else {
            yPos--;
        }

    } else if (mouseXpos >= hexagons[yPos][xPos].point1.x && mouseXpos <= hexagons[yPos][xPos].point2.x) {
        if (mouseYpos <= a1 * mouseXpos + b1) {
            if (yPos > 4 && yPos < 13 && yPos % 2 != 0) {
                yPos--;
            } else if (yPos >= 13) {
                yPos--;
            } else {
                yPos--;
                xPos--;
            }
        }
    } else if (mouseXpos < hexagons[yPos][xPos].point1.x) {
        yPos--;
    }
    if (xPos == 5 || xPos+yPos==17) {
        xPos--;
    }
    return {yPos, xPos};
}

auto GamePanel::manageGamePanel(int mouseXPosition, int mouseYPosition, sf::RenderWindow &window) -> void {
    if (!isMoving) {
        if (!choosingMove) {
            // here we're selecting a pawn with which we want to move
            auto location = focusedHexagon(mouseXPosition, mouseYPosition);
            if (location.x >= 0 && location.y >= 0) {
                if (!hexagons[location.x][location.y].isEmpty &&
                    hexagons[location.x][location.y].currentPlayer == currentPlayerMove) {
                    // we're setting a pawn as focus
                    rowOfFocus = location.x;
                    colOfFocus = location.y;
                    choices = showPossibilities(currentPlayerMove, location.x, location.y, window);
                    for (int k = 0; k < choices.size(); k++) {
                        if (choices[k].isFar) hexagons[choices[k].end.x][choices[k].end.y].isPossibilityToMoveFar = true;
                        else hexagons[choices[k].end.x][choices[k].end.y].isPossibilityToMoveClose = true;
                    }
                    choosingMove = true;
                }
            }
        } else if (choosingMove) {
            // checking in case someone wants to change the pawn with which he moves
            auto location = focusedHexagon(mouseXPosition, mouseYPosition);
            if (location.x >= 0 && location.y >= 0) {
                if (hexagons[location.x][location.y].currentPlayer == currentPlayerMove) {
                    // clearing the previous possibilities
                    for (int k = 0; k < choices.size(); k++) {
                        if (choices[k].isFar) hexagons[choices[k].end.x][choices[k].end.y].isPossibilityToMoveFar = false;
                        else hexagons[choices[k].end.x][choices[k].end.y].isPossibilityToMoveClose = false;
                    }
                    rowOfFocus = location.x;
                    colOfFocus = location.y;
                    choices = showPossibilities(currentPlayerMove, location.x, location.y, window);
                    for (int k = 0; k < choices.size(); k++) {
                        if (choices[k].isFar) hexagons[choices[k].end.x][choices[k].end.y].isPossibilityToMoveFar = true;
                        else hexagons[choices[k].end.x][choices[k].end.y].isPossibilityToMoveClose = true;
                    }
                }
            }
            // here we're choosing the actual move
            for (int i = 0; i < choices.size(); i++) {
                if (hexagons[choices[i].end.x][choices[i].end.y].isCursorIn(mouseXPosition, mouseYPosition)) {
                    if (hexagons[choices[i].end.x][choices[i].end.y].isUsable &&
                        hexagons[choices[i].end.x][choices[i].end.y].isEmpty) {
                        if (choices[i].isFar) {
                            moveFar(currentPlayerMove, rowOfFocus, colOfFocus,
                                    choices[i].end.x, choices[i].end.y);
                        } else {
                            moveClose(currentPlayerMove, rowOfFocus, colOfFocus, choices[i].end.x, choices[i].end.y);
                        }
                        if (currentPlayerMove == 1) currentPlayerMove = 2;
                        else currentPlayerMove = 1;
                        choosingMove = false;
                        for (int k = 0; k < choices.size(); k++) {
                            if (choices[k].isFar) hexagons[choices[k].end.x][choices[k].end.y].isPossibilityToMoveFar = false;
                            else hexagons[choices[k].end.x][choices[k].end.y].isPossibilityToMoveClose = false;
                        }
                    }
                }
            }
        }
    }
}

auto GamePanel::manageFinishPanel(int mouseXPosition, int mouseYPosition) -> void {
    if (bonusButton.getGlobalBounds().contains(mouseXPosition, mouseYPosition)) {
        fun = true;
    }
}

auto GamePanel::manageFiles() -> void {
    if (!endOfGameScreenOn) {
        saveToFile("Old_Games\\" + gameName);
    } else {
        // if the game belongs in the top 5 thats where we save it
        bestGames(gameName);
        // if we've loaded a game that wasnt over and we finished it thats where we delete it from the file
        auto dirIT = std::filesystem::directory_iterator("Old_Games");
        for (const auto &entry: dirIT) {
            if (entry.path().filename() == gameName) {
                std::filesystem::remove(entry);
            }
        }
    }
}
