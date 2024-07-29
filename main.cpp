/*
 * File: main.cpp
 * Author: YOUR NAME HERE
 * Created on DATE AND TIME HERE
 * Purpose: Connect 4 Game
 */

// System Libraries
#include <iostream>  // Input/Output Stream Library
#include <iomanip>   // Formatting Library
#include <ctime>     // Unique Seed Value Library
#include <cstdlib>   // Random Value Library
#include <string>    // String Library
#include <fstream>   // File I/O
#include <cmath>     // Math Library
using namespace std;

// Function Prototypes
void initializeBoard(char[][7], int, int);
void printBoard(const char[][7], int, int);
bool makeMove(char[][7], int, char, int);
bool checkWin(const char[][7], char, int, int);
bool checkFull(const char[][7], int, int);
void saveGame(const char[][7], const string&, int, int);
void loadGame(char[][7], const string&, int, int);
void welcomeMsg();
void mainMenu(bool&, bool&);
void gameLoop(char[][7], int, int, char&, bool&, bool&);
void displayStats(int, int, int, int);
void saveStats(int, int, int, int);
void loadStats(int&, int&, int&, int&);

// Program Execution Begins Here
int main(int argc, char **argv) {
    // Declare all Variables Here
    const int ROWS = 6;
    const int COLS = 7;
    char board[ROWS][COLS];
    string filename = "connect4_save.dat";
    char player = '1';  // Player 1 represented by '1' and Player 2 represented by '2'
    bool gameWon = false, boardFull = false;
    int gamesPlayed = 0, gamesDrawn = 0, player1Wins = 0, player2Wins = 0;
    bool loadPrevGame = false;
    bool viewStats = false;

    // Load Statistics
    loadStats(gamesPlayed, player1Wins, player2Wins, gamesDrawn);

    // Display Welcome Message
    welcomeMsg();

    // Wait for Enter to be pressed
    cout << "Press Enter TWICE to start...";
    cin.ignore();
    cin.get();

    // Main Menu
    mainMenu(loadPrevGame, viewStats);

    if (viewStats) {
        displayStats(gamesPlayed, gamesDrawn, player1Wins, player2Wins);
        char returnChoice;
        cout << "Return to menu? (y/n): ";
        cin >> returnChoice;
        if (returnChoice == 'y' || returnChoice == 'Y') {
            mainMenu(loadPrevGame, viewStats);
        } else {
            return 0;
        }
    }

    // Load previous game if chosen
    if (loadPrevGame) {
        loadGame(board, filename, ROWS, COLS);
    } else {
        // Initialize the game board
        initializeBoard(board, ROWS, COLS);
    }

    // Game Loop
    gameLoop(board, ROWS, COLS, player, gameWon, boardFull);

    // Update Statistics
    gamesPlayed++;
    if (gameWon) {
        if (player == '1') {
            player1Wins++;
        } else {
            player2Wins++;
        }
    } else {
        gamesDrawn++;
    }

    // Save Statistics
    saveStats(gamesPlayed, player1Wins, player2Wins, gamesDrawn);

    // Save the game state
    saveGame(board, filename, ROWS, COLS);

    return 0;
}

void welcomeMsg() {
    cout << "================================" << endl;
    cout << "      * * * WELCOME! * * *" << endl;
    cout << "        * * * TO * * *" << endl;
    cout << "   * * * CONNECT FOUR * * *" << endl;
    cout << "=================================" << endl;
}

void mainMenu(bool &loadPrevGame, bool &viewStats) {
    int choice;
    cout << "Main Menu:" << endl;
    cout << "1. Start New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. View Stats" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            loadPrevGame = false;
            return;
        case 2:
            loadPrevGame = true;
            return;
        case 3:
            viewStats = true;
            return;
        case 4:
            exit(0);
        default:
            cout << "Invalid choice. Please try again." << endl;
            mainMenu(loadPrevGame, viewStats);
    }
}

void initializeBoard(char board[][7], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i][j] = 'O';
        }
    }
}

void printBoard(const char board[][7], int rows, int cols) {
    cout << "                    _____________________________" << endl;
    for (int i = 0; i < rows; ++i) {
        cout << "                    | ";
        for (int j = 0; j < cols; ++j) {
            cout << board[i][j] << " | ";
        }
        cout << endl;
    }
    cout << "          __________|___|___|___|___|___|___|___|__________" << endl;
    cout << "                      1   2   3   4   5   6   7            " << endl;
}

bool makeMove(char board[][7], int col, char player, int rows) {
    for (int i = rows - 1; i >= 0; --i) {
        if (board[i][col] == 'O') {
            board[i][col] = player;
            return true;
        }
    }
    return false;
}

bool checkWin(const char board[][7], char player, int rows, int cols) {
    // Check horizontal
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols - 3; ++j) {
            if (board[i][j] == player && board[i][j + 1] == player &&
                board[i][j + 2] == player && board[i][j + 3] == player) {
                return true;
            }
        }
    }

    // Check vertical
    for (int i = 0; i < rows - 3; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j] == player && board[i + 1][j] == player &&
                board[i + 2][j] == player && board[i + 3][j] == player) {
                return true;
            }
        }
    }

    // Check diagonal (bottom-left to top-right)
    for (int i = 3; i < rows; ++i) {
        for (int j = 0; j < cols - 3; ++j) {
            if (board[i][j] == player && board[i - 1][j + 1] == player &&
                board[i - 2][j + 2] == player && board[i - 3][j + 3] == player) {
                return true;
            }
        }
    }

    // Check diagonal (top-left to bottom-right)
    for (int i = 0; i < rows - 3; ++i) {
        for (int j = 0; j < cols - 3; ++j) {
            if (board[i][j] == player && board[i + 1][j + 1] == player &&
                board[i + 2][j + 2] == player && board[i + 3][j + 3] == player) {
                return true;
            }
        }
    }

    return false;
}

bool checkFull(const char board[][7], int rows, int cols) {
    for (int j = 0; j < cols; ++j) {
        if (board[0][j] == 'O') {
            return false;
        }
    }
    return true;
}

void saveGame(const char board[][7], const string &filename, int rows, int cols) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                outFile << board[i][j] << ' ';
            }
            outFile << endl;
        }
        outFile.close();
    } else {
        cout << "Unable to open file for saving." << endl;
    }
}

void loadGame(char board[][7], const string &filename, int rows, int cols) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                inFile >> board[i][j];
            }
        }
        inFile.close();
    } else {
        cout << "Unable to open file for loading." << endl;
    }
}

void gameLoop(char board[][7], int rows, int cols, char &player, bool &gameWon, bool &boardFull) {
    while (!gameWon && !boardFull) {
        printBoard(board, rows, cols);
        cout << "Player " << player << ", enter a column (1-7): ";
        int colChoice;
        cin >> colChoice;
        colChoice--;

        while (colChoice < 0 || colChoice >= cols || !makeMove(board, colChoice, player, rows)) {
            cout << "Invalid move. Try again: ";
            cin >> colChoice;
            colChoice--; 
        }

        gameWon = checkWin(board, player, rows, cols);
        if (!gameWon) {
            boardFull = checkFull(board, rows, cols);
            player = (player == '1') ? '2' : '1';
        }
    }

    printBoard(board, rows, cols);

    if (gameWon) {
        cout << "Player " << player << " wins!" << endl;
    } else {
        cout << "The game is a draw!" << endl;
    }
}

void displayStats(int gamesPlayed, int gamesDrawn, int player1Wins, int player2Wins) {
    cout << "Game Statistics:" << endl;
    cout << "Total Games Played: " << gamesPlayed << endl;
    cout << "Total Games Drawn: " << gamesDrawn << endl;
    cout << "Total Games Won by Player 1: " << player1Wins << endl;
    cout << "Total Games Won by Player 2: " << player2Wins << endl;
}

void saveStats(int gamesPlayed, int player1Wins, int player2Wins, int gamesDrawn) {
    ofstream outFile("connect4_stats.dat");
    if (outFile.is_open()) {
        outFile << gamesPlayed << ' ' << player1Wins << ' ' << player2Wins << ' ' << gamesDrawn << endl;
        outFile.close();
    } else {
        cout << "Unable to open file for saving statistics." << endl;
    }
}

void loadStats(int &gamesPlayed, int &player1Wins, int &player2Wins, int &gamesDrawn) {
    ifstream inFile("connect4_stats.dat");
    if (inFile.is_open()) {
        inFile >> gamesPlayed >> player1Wins >> player2Wins >> gamesDrawn;
        inFile.close();
    } else {
        gamesPlayed = 0;
        player1Wins = 0;
        player2Wins = 0;
        gamesDrawn = 0;
    }
}
