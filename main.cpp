/* 
 * File: main.cpp
 * Author: YOUR NAME HERE
 * Created on DATE AND TIME HERE
 * Purpose: Connect 4 Game (Simplified Version)
 */

// System Libraries
#include <iostream>  // Input/Output Stream Library
#include <iomanip>   // Formatting Library
#include <ctime>     // Unique Seed Value Library
#include <cstdlib>   // Random Value Library
#include <string>    // String Library
using namespace std;

// Function Prototypes
void initializeBoard(char[][7], int, int);
void printBoard(const char[][7], int, int);
bool makeMove(char[][7], int, char, int);
bool checkWin(const char[][7], char, int, int);
bool checkFull(const char[][7], int, int);

// Program Execution Begins Here
int main(int argc, char **argv) {
    // Declare all Variables Here
    const int ROWS = 6;
    const int COLS = 7;
    char board[ROWS][COLS];
    char player = '1';  // Player 1 represented by '1' and Player 2 represented by '2'
    bool gameWon = false, boardFull = false;
    int colChoice;

    // Set the Random Number Seed
    srand(static_cast<unsigned int>(time(0)));

    // Initialize the game board
    initializeBoard(board, ROWS, COLS);
    // Game Loop
    while (!gameWon && !boardFull) {
        printBoard(board, ROWS, COLS);
        cout << "Player " << player << ", enter a column (1-7): ";
        cin >> colChoice;

        // Adjust for zero-based indexing
        colChoice--;

        while (colChoice < 0 || colChoice >= COLS || !makeMove(board, colChoice, player, ROWS)) {
            cout << "Invalid move. Try again: ";
            cin >> colChoice;
            colChoice--;  // Adjust for zero-based indexing
        }

        gameWon = checkWin(board, player, ROWS, COLS);
        if (!gameWon) {
            boardFull = checkFull(board, ROWS, COLS);
            player = (player == '1') ? '2' : '1';
        }
    }

    printBoard(board, ROWS, COLS);

    if (gameWon) {
        cout << "Player " << player << " wins!" << endl;
    } else {
        cout << "The game is a draw!" << endl;
    }

    return 0;
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
