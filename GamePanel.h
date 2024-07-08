#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Hexagon.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Move.h"
#include "SFML/Graphics/CircleShape.hpp"


enum GameMode{
    vsPlayer, vsAI
};
/**
\brief This structure represents the game panel and holds all the logical functions
@param gameMap a vector<int> that has as many rows as the map and each value represents the number of columns; based on this, we build the next parameter
@param hexagons one of the most important fields, it represents all the fields on the board
@param gameName used for saving or reading from a file
@param radius the side length of one of the 6 equilateral triangles that the hexagon is built from (important for calculations in some functions)
@param sideLength the height of the hexagon calculated based on the radius, I made this field for readability and convenience
@param startPanelOn represents the initial state of the game, based on it and a few subsequent ones, another function drawing our GamePanel is called
@param gameChoiceScreenOn represents the state of the game mode selection
@param endOfGameScreenOn after the game is over, we set this variable to true and display the end panel
@param bestGamesScreeOn represents the state of the game panel during showing best games
@param gamePanelOn represents the state of the game panel when we actually play the game
@param wrongInput used in the readFromFile and saveToFile methods in case the user enters an incorrect name
@param choosingMove during the actual game, the user either selects the pawn they want to move or the move for the previously selected pawn, this variable represents that
@param isMoving after every move there is an animation and during that animation we don't want anything else to happen this param takes care of it
@param counterForMove when the animation of a move is happening this counts the frames after it reaches a specific point animation ends
@param end this is used for the animation holds the place to which a hexagon moves
@param fun at the very end of the game, there is a little surprise after clicking on it
@param currentPlayerMove represents which player has the move in the current turn
@param scoreOne number of pawns owned by player 1
@param scoreTwo number of pawns owned by player 2
@param mode vsPlayer indicates playing vs a player, vsAI indicated playing vs AI
@param score1 graphical component corresponding to scoreOne
@param score2 graphical component corresponding to scoreTwo
@param currentMove graphical component corresponding to currentPlayerMove
@param font universal sf::Text font loaded from a file for all graphical components
@param pawnMoving graphical component which we use in the moving animation
@param vsPlayerBox used in the ChoicePanel, corresponds to the figure with VS PLAYER
@param vsAIBox used in the ChoicePanel, corresponds to the figure with VS AI
@param newGameBox used in the StartPanel, corresponds to New Game
@param loadGameBox used in the StartPanel, corresponds to Load Game
@param bestGamesBox used in the StartPanel, corresponds to Best Games
@param backButton used in the Best Games to return to the Start Panel
@param bonusButton used at the very end, related to a surprise
*/
struct GamePanel{
    std::vector<int> gameMap;
    std::vector<std::vector<Hexagon>> hexagons;
    std::string gameName;
    float radius;
    float sideLength;
    bool startPanelOn;
    bool gameChoiceScreenOn;
    bool endOfGameScreenOn;
    bool bestGamesScreenOn;
    bool gamePanelOn;
    bool wrongInput;
    bool choosingMove;
    bool isMoving;
    int counterForMove;
    sf::Vector2f end;
    bool fun;
    int currentPlayerMove;
    int scoreOne;
    int scoreTwo;
    GameMode mode;
    int rowOfFocus;
    int colOfFocus;
    std::vector<Move> choices;
    sf::Text score1;
    sf::Text score2;
    sf::Text currentMove;
    sf::Font font;
    sf::CircleShape pawnMoving;
    sf::RectangleShape vsPlayerBox;
    sf::RectangleShape vsAIBox;
    sf::RectangleShape newGameBox;
    sf::RectangleShape loadGameBox;
    sf::RectangleShape bestGamesBox;
    sf::RectangleShape backButton;
    sf::RectangleShape bonusButton;

/**
 * \brief This function clones a pawn and moves it to the specified location using the last 2 parameters
 * @param player indicates the player who is moving
 * @param startRow the x coordinate of the pawn being moved
 * @param startCol the y coordinate of the pawn being moved
 * @param finishRow the x coordinate of the field the pawn is moving to
 * @param finishCol the y coordinate of the field the pawn is moving to
 */
    auto moveClose(int player, int startRow, int startCol, int finishRow, int finishCol) -> void;

/**
 * \brief This function moves a pawn to the specified location using the last 2 parameters
 * @param player indicates the player who is moving
 * @param startRow the x coordinate of the pawn being moved
 * @param startCol the y coordinate of the pawn being moved
 * @param finishRow the x coordinate of the field the pawn is moving to
 * @param finishCol the y coordinate of the field the pawn is moving to
 */
    auto moveFar(int player, int startRow, int startCol, int finishRow, int finishCol) -> void;

/**
 * \brief This function represents the move made by the AI by looking at the current best move using the currentBestMove function.
 * We calculate the best move using the CurrentBestMove() function and assign it to the move variable, then we check whether move.isFar=true or not and call moveClose/moveFar accordingly.
 */
    auto aiMove() -> void;

/**
 * \brief This function is used to show all possible moves and return a std::vector<Move> for further calculations.
 * The function checks the distances of specific hexagons from the hexagon at index (row, col) and if they meet certain conditions, it changes their possibilityToMoveClose/Far field.
 * @param player depending on the player number, the moves may be different, hence this parameter
 * @param row the x index in the hexagons list
 * @param col the y index in the hexagons list
 * @param window the window on which the hexagon will be drawn
 * @return returns a list of possible moves that will be needed to make the actual move
 */
    auto showPossibilities(int player, int row, int col, sf::RenderWindow& window) -> std::vector<Move>;

/**
 * \brief We use convertAround() immediately after making a move for the hexagon at index (row, col) and around it, we change all opposing hexagons to the same as the one at index (row, col).
 * @param player for which player this function should be executed
 * @param row the x index of the hexagon
 * @param col the y index of the hexagon
 */
    auto convertAround(int player, int row, int col) -> void;


    /**
     * \brief After each turn, this function is called to check if the game is over.
     * It checks the following conditions:
     * 1. Does any player have 0 pieces?
     * 2. Does any player have 0 possible moves?
     * 3. Is the total number of pieces of both players equal to 58 (total number of positions on the board)?
     * Returns true if any of these conditions is met and the game is over.
     */
    auto checkIfOver() -> bool;


   /**
    * \brief Used by the checkIfOver() function to check the number of possible moves for a given piece.
    * @param row  x index of the hexagon for which we count the number of possibilities.
    * @param col y index of the hexagon for which we count the number of possibilities.
    * @return  Returns the number of possibilities.
    */
    auto showAmountofPossibilities(int row, int col) const -> int;


    // Initializes all fields and builds the entire game board.
    GamePanel();

    auto focusedHexagon(int mouseXpos, int mouseYpos) const -> sf::Vector2i;


    /**
     * \brief Draws the game mode selection panel.
     * @param window the window to draw on.
     */
    auto drawChoicePanel(sf::RenderWindow &window) const->void;


    /**
     * \brief Draws the actual game panel.
     * @param window the window to draw on.
     */
    auto drawGamePanel(sf::RenderWindow &window) -> void;


    /**
     * \brief Draws the end game panel.
     * @param window the window to draw on.
     */
    auto drawFinishPanel(sf::RenderWindow &window) -> void;


    /**
     * \brief Draws the panel showing the best games.
     * @param window the window to draw on.
     */
    auto drawBestGamesPanel(sf::RenderWindow &window) -> void;


    /**
     * \brief If the game is not over and the application is closed, this function saves the current game state to a file.
     * The saved file includes:
     * - Player's move
     * - End of Game Screen On
     * - Game mode
     * - Score of player one
     * - Score of player two
     * Followed by the details of each hexagon and its properties:
     * - Index x
     * - Index y
     * - Is usable
     * - Is empty
     * - Current player
     * @param fileName the file path to save the game state.
     */
    auto saveToFile(const std::string& fileName) -> void;

    /**
     * \brief Called when loading the game from a file.
     * @param fileName the file path to read the game state from.
     */
    auto readFromFile(const std::string& fileName) -> void;


    /**
     * \brief Draws the start panel.
     * @param window window: the window to draw on.
     */
    auto drawStartPanel(sf::RenderWindow &window) const -> void;


    /**
     * \brief Called at the end of the game to check if the game is better than any of the top 5 best games.
     * If it is, it saves the game to the "bestGames" file.
     * @param gName the name of the game to be saved.
     */
    auto bestGames(const std::string& gName) const -> void;


    /**
     * \brief used to calculate the best move in a specific turn, it is used in AI mode during its move
     * @return Move is an entity that holds from where to where
     */
    auto currentBestMove() const -> Move;


    /**
     * \brief function thats actually called in main decides which panel to draw
     * @param window window on which we'll draw
     */
    auto draw(sf::RenderWindow& window)->void;

    /**
     * \brief we're using this function just before the program ends it either saves the file in not over deletes the file if need be and checks if we should add it to bestGames
     * @param gameName name of the Game user inserted
     */
    auto manageFiles() -> void;

    /**
     * \brief we want to manage everything that happens during the start panel
     * start panel shows 3 boxes depending on the parameters we interact with them
     * @param mouseXPosition
     * @param mouseYPosition
     */
    auto manageStartPanel(int mouseXPosition, int mouseYPosition)->void;

    /**
     * \brief we manage every interaction with graphic components during bestGames panel
     * @param mouseXPosition
     * @param mouseYPosition
     */
    auto manageBestGamesPanel(int mouseXPosition, int mouseYPosition)->void;

    /**
     * \brief manages every interaction with buttons on the panel of choosing game mode
     * @param mouseXPosition
     * @param mouseYPosition
     */
    auto manageGameModeChoicePanel(int mouseXPosition, int mouseYPosition)->void;

    /**
     * \brief manages every interaction during actual game Panel such as moving, selecting a hexagon focus etc
     * @param mouseXPosition
     * @param mouseYPosition
     * @param window window to which we draw moves etc
     */
    auto manageGamePanel(int mouseXPosition, int mouseYPosition,sf::RenderWindow& window)->void;

    /**
     *  \brief manages interactions with the last Panel (just the button in the bottom)
     * @param mouseXPosition
     * @param mouseYPosition
     */
    auto manageFinishPanel(int mouseXPosition, int mouseYPosition)->void;

};

