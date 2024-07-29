/*
 * File: main.cpp
 * Author: YOUR NAME HERE
 * Created on DATE AND TIME HERE
 * Purpose: Connect 4 Game with Nicknames and Leaderboard
 */

// System Libraries
#include <iostream>  // Input/Output Stream Library
#include <iomanip>   // Formatting Library
#include <ctime>     // Unique Seed Value Library
#include <cstdlib>   // Random Value Library
#include <string>    // String Library
#include <fstream>   // File I/O
#include <cmath>     // Math Library
#include <vector>    // STL Vectors
#include <utility>   // pair
#include <algorithm> // sort
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
void mainMenu(bool&, bool&, bool&);
void gameLoop(char[][7], int, int, char&, bool&, bool&, string&, string&, char&, char&);
void displayStats(int, int, int, int);
void saveStats(int, int, int, int);
void loadStats(int&, int&, int&, int&);
void displayLeaderboard();
void saveLeaderboard(const vector<pair<string, int> >&);
void loadLeaderboard(vector<pair<string, int> >&);
void getPlayerDetails(string&, string&, char&, char&);
void updateLeaderboard(const string&, const string&, bool);
bool comparePairs(const pair<string, int> &, const pair<string, int> &);

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
    bool viewLeaderboard = false;
    string player1, player2;
    char player1Char = '1', player2Char = '2';

    // Load Statistics
    loadStats(gamesPlayed, player1Wins, player2Wins, gamesDrawn);

    // Display Welcome Message
    welcomeMsg();

    // Wait for Enter to be pressed
    cout << "Press Enter TWICE to start...";
    cin.ignore();
    cin.get();

    // Main Menu
    mainMenu(loadPrevGame, viewStats, viewLeaderboard);

    if (viewStats) {
        displayStats(gamesPlayed, gamesDrawn, player1Wins, player2Wins);
        char returnChoice;
        cout << "Return to menu? (y/n): ";
        cin >> returnChoice;
        if (returnChoice == 'y' || returnChoice == 'Y') {
            mainMenu(loadPrevGame, viewStats, viewLeaderboard);
        } else {
            return 0;
        }
    }

    if (viewLeaderboard) {
        displayLeaderboard();
        char returnChoice;
        cout << "Return to menu? (y/n): ";
        cin >> returnChoice;
        if (returnChoice == 'y' || returnChoice == 'Y') {
            mainMenu(loadPrevGame, viewStats, viewLeaderboard);
        } else {
            return 0;
        }
    }

    // Get player details
    getPlayerDetails(player1, player2, player1Char, player2Char);

    // Load previous game if chosen
    if (loadPrevGame) {
        loadGame(board, filename, ROWS, COLS);
    } else {
        // Initialize the game board
        initializeBoard(board, ROWS, COLS);
    }

    // Game Loop
    gameLoop(board, ROWS, COLS, player, gameWon, boardFull, player1, player2, player1Char, player2Char);

    // Update Statistics
    gamesPlayed++;
    if (gameWon) {
        if (player == player1Char) {
            player1Wins++;
            updateLeaderboard(player1, player2, true);
        } else {
            player2Wins++;
            updateLeaderboard(player1, player2, false);
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

void mainMenu(bool &loadPrevGame, bool &viewStats, bool &viewLeaderboard) {
    int choice;
    cout << "Main Menu:" << endl;
    cout << "1. Start New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. View Stats" << endl;
    cout << "4. View Leaderboard" << endl;
    cout << "5. Exit" << endl;
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
            viewLeaderboard = true;
            return;
        case 5:
            exit(0);
        default:
            cout << "Invalid choice. Please try again." << endl;
            mainMenu(loadPrevGame, viewStats, viewLeaderboard);
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

void gameLoop(char board[][7], int rows, int cols, char &player, bool &gameWon, bool &boardFull, string &player1, string &player2, char &player1Char, char &player2Char) {
    while (!gameWon && !boardFull) {
        printBoard(board, rows, cols);
        cout << (player == player1Char ? player1 : player2) << " (" << (player == player1Char ? "Player 1" : "Player 2") << "), enter a column (1-7): ";
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
            player = (player == player1Char) ? player2Char : player1Char;
        }
    }

    printBoard(board, rows, cols);

    if (gameWon) {
        cout << (player == player1Char ? player1 : player2) << " (" << (player == player1Char ? "Player 1" : "Player 2") << ") wins!" << endl;
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

bool comparePairs(const pair<string, int> &a, const pair<string, int> &b) {
    return a.second > b.second;
}

void displayLeaderboard() {
    vector<pair<string, int> > leaderboard;
    loadLeaderboard(leaderboard);

    // Sort leaderboard by number of wins in descending order
    sort(leaderboard.begin(), leaderboard.end(), comparePairs);

    cout << "Leaderboard:" << endl;
    cout << "Rank  Player          Wins" << endl;
    cout << "----------------------------" << endl;
    for (size_t i = 0; i < leaderboard.size(); ++i) {
        cout << setw(4) << (i + 1) << "  " << setw(14) << leaderboard[i].first << "  " << leaderboard[i].second << endl;
    }
}

void saveLeaderboard(const vector<pair<string, int> > &leaderboard) {
    ofstream outFile("connect4_leaderboard.dat");
    if (outFile.is_open()) {
        for (size_t i = 0; i < leaderboard.size(); ++i) {
            outFile << leaderboard[i].first << ' ' << leaderboard[i].second << endl;
        }
        outFile.close();
    } else {
        cout << "Unable to open file for saving leaderboard." << endl;
    }
}

void loadLeaderboard(vector<pair<string, int> > &leaderboard) {
    ifstream inFile("connect4_leaderboard.dat");
    if (inFile.is_open()) {
        leaderboard.clear();
        string playerName;
        int wins;
        while (inFile >> playerName >> wins) {
            leaderboard.push_back(make_pair(playerName, wins));
        }
        inFile.close();
    }
}

void getPlayerDetails(string &player1, string &player2, char &player1Char, char &player2Char) {
    cout << "Enter Player 1 nickname: ";
    cin >> player1;
    cout << "Enter Player 1 character (default '1'): ";
    cin.ignore();
    player1Char = cin.get();
    if (player1Char == '\n' || player1Char == ' ') {
        player1Char = '1';
    }

    cout << "Enter Player 2 nickname: ";
    cin >> player2;
    cout << "Enter Player 2 character (default '2'): ";
    cin.ignore();
    player2Char = cin.get();
    if (player2Char == '\n' || player2Char == ' ') {
        player2Char = '2';
    }
}

void updateLeaderboard(const string &player1, const string &player2, bool player1Won) {
    vector<pair<string, int> > leaderboard;
    loadLeaderboard(leaderboard);

    bool player1Found = false, player2Found = false;
    for (size_t i = 0; i < leaderboard.size(); ++i) {
        if (leaderboard[i].first == player1) {
            if (player1Won) leaderboard[i].second++;
            player1Found = true;
        }
        if (leaderboard[i].first == player2) {
            if (!player1Won) leaderboard[i].second++;
            player2Found = true;
        }
    }

    if (!player1Found && player1Won) {
        leaderboard.push_back(make_pair(player1, 1));
    } else if (!player1Found) {
        leaderboard.push_back(make_pair(player1, 0));
    }

    if (!player2Found && !player1Won) {
        leaderboard.push_back(make_pair(player2, 1));
    } else if (!player2Found) {
        leaderboard.push_back(make_pair(player2, 0));
    }

    saveLeaderboard(leaderboard);
}
