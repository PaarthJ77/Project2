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
using namespace std;

// Function Prototypes
void initBrd(char[][7], int, int);
void printBrd(const char[][7], int, int);
bool makeMove(char[][7], int, char, int);
bool checkWin(const char[][7], char, int, int);
bool checkFull(const char[][7], int, int);
void saveGme(const char[][7], const string&, int, int);
void loadGme(char[][7], const string&, int, int);
void welcomeMsg();

int main(int argc, char **argv) {
    const int ROWS = 6;
    const int COLS = 7;
    char board[ROWS][COLS];
    string fname = "connect4.dat";
    char player = '1';
    bool gmeWon = false, brdFull = false;
    int colChc;
    char loadChc;

    srand(static_cast<unsigned int>(time(0)));

    // Display welcome message
    welcomeMsg();

    // Initialize the game board
    initBrd(board, ROWS, COLS);

    // Ask if user wants to load a previous game
    cout << "Do you want to load a previous game? (y/n): ";
    cin >> loadChc;

    if (loadChc == 'y' || loadChc == 'Y') {
        loadGme(board, fname, ROWS, COLS);
    }

    // Game Loop
    while (!gmeWon && !brdFull) {
        printBrd(board, ROWS, COLS);
        cout << "Player " << player << ", enter a column (1-7): ";
        cin >> colChc;

        colChc--;

        while (colChc < 0 || colChc >= COLS || !makeMove(board, colChc, player, ROWS)) {
            cout << "Invalid move. Try again: ";
            cin >> colChc;
            colChc--;
        }

        gmeWon = checkWin(board, player, ROWS, COLS);
        if (!gmeWon) {
            brdFull = checkFull(board, ROWS, COLS);
            player = (player == '1') ? '2' : '1';
        }
    }

    printBrd(board, ROWS, COLS);

    if (gmeWon) {
        cout << "Player " << player << " wins!" << endl;
    } else {
        cout << "The game is a draw!" << endl;
    }

    saveGme(board, fname, ROWS, COLS);

    return 0;
}

void initBrd(char brd[][7], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            brd[i][j] = 'O';
        }
    }
}

void printBrd(const char brd[][7], int rows, int cols) {
    cout << "                    _____________________________" << endl;
    for (int i = 0; i < rows; ++i) {
        cout << "                    | ";
        for (int j = 0; j < cols; ++j) {
            cout << "\033[1m" << brd[i][j] << "\033[0m" << " | ";  // Bold characters
        }
        cout << endl;
    }
    cout << "          __________|___|___|___|___|___|___|___|__________" << endl;
    cout << "                      1   2   3   4   5   6   7            " << endl;
}

bool makeMove(char brd[][7], int col, char plyr, int rows) {
    for (int i = rows - 1; i >= 0; --i) {
        if (brd[i][col] == 'O') {
            brd[i][col] = plyr;
            return true;
        }
    }
    return false;
}

bool checkWin(const char brd[][7], char plyr, int rows, int cols) {
    // Check horizontal
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols - 3; ++j) {
            if (brd[i][j] == plyr && brd[i][j + 1] == plyr &&
                brd[i][j + 2] == plyr && brd[i][j + 3] == plyr) {
                return true;
            }
        }
    }

    // Check vertical
    for (int i = 0; i < rows - 3; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (brd[i][j] == plyr && brd[i + 1][j] == plyr &&
                brd[i + 2][j] == plyr && brd[i + 3][j] == plyr) {
                return true;
            }
        }
    }

    // Check diagonal (bottom-left to top-right)
    for (int i = 3; i < rows; ++i) {
        for (int j = 0; j < cols - 3; ++j) {
            if (brd[i][j] == plyr && brd[i - 1][j + 1] == plyr &&
                brd[i - 2][j + 2] == plyr && brd[i - 3][j + 3] == plyr) {
                return true;
            }
        }
    }

    // Check diagonal (top-left to bottom-right)
    for (int i = 0; i < rows - 3; ++i) {
        for (int j = 0; j < cols - 3; ++j) {
            if (brd[i][j] == plyr && brd[i + 1][j + 1] == plyr &&
                brd[i + 2][j + 2] == plyr && brd[i + 3][j + 3] == plyr) {
                return true;
            }
        }
    }

    return false;
}

bool checkFull(const char brd[][7], int rows, int cols) {
    for (int j = 0; j < cols; ++j) {
        if (brd[0][j] == 'O') {
            return false;
        }
    }
    return true;
}

void saveGme(const char brd[][7], const string &fname, int rows, int cols) {
    ofstream outFle(fname);
    if (outFle.is_open()) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                outFle << brd[i][j] << ' ';
            }
            outFle << endl;
        }
        outFle.close();
    } else {
        cout << "Unable to open file for saving." << endl;
    }
}

void loadGme(char brd[][7], const string &fname, int rows, int cols) {
    ifstream inFle(fname);
    if (inFle.is_open()) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                inFle >> brd[i][j];
            }
        }
        inFle.close();
    } else {
        cout << "Unable to open file for loading." << endl;
    }
}

void welcomeMsg() {
    cout << "================================" << endl;
    cout << "      * * * WELCOME! * * *" << endl;
    cout << "        * * * TO * * *" << endl;
    cout << "   * * * CONNECT FOUR * * *" << endl;
    cout << "=================================" << endl;
}

