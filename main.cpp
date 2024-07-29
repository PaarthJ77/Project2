/* 
 * File: main.cpp
 * Author: YOUR NAME HERE
 * Created on DATE AND TIME HERE
 * Purpose: Connect 4 Game (Simplified Version)
 */

// System Libraries
#include <iostream>  // Input/Output Stream Library
#include <iomanip>   // Formatting Library
using namespace std;

// Function Prototypes
void initializeBoard(char[][7], int, int);
void printBoard(const char[][7], int, int);

// Program Execution Begins Here
int main(int argc, char **argv) {
    // Declare all Variables Here
    const int ROWS = 6;
    const int COLS = 7;
    char board[ROWS][COLS];

    // Initialize the game board
    initializeBoard(board, ROWS, COLS);

    // Print the game board
    printBoard(board, ROWS, COLS);

    // Exit
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
