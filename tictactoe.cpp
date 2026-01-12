#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

class TicTacToe {
private:
    vector<vector<char>> board;
    char currentPlayer;
    int gameMode;
    
    void initBoard() {
        board = vector<vector<char>>(3, vector<char>(3, ' '));
        currentPlayer = 'X';
    }
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void displayTitle() {
        clearScreen();
        cout << "\n";
        cout << "========================================\n";
        cout << "|                                      |\n";
        cout << "|        TIC-TAC-TOE GAME              |\n";
        cout << "|                                      |\n";
        cout << "========================================\n\n";
    }
    
    void displayBoard() {
        cout << "\n";
        cout << "     +---+---+---+\n";
        for (int i = 0; i < 3; i++) {
            cout << "     |";
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ')
                    cout << "   ";
                else
                    cout << " " << board[i][j] << " ";
                
                cout << "|";
            }
            cout << "\n";
            if (i < 2)
                cout << "     +---+---+---+\n";
        }
        cout << "     +---+---+---+\n\n";
        
        cout << "  Position Guide:\n";
        cout << "     +---+---+---+\n";
        int pos = 1;
        for (int i = 0; i < 3; i++) {
            cout << "     |";
            for (int j = 0; j < 3; j++) {
                cout << " " << pos++ << " |";
            }
            cout << "\n";
            if (i < 2)
                cout << "     +---+---+---+\n";
        }
        cout << "     +---+---+---+\n\n";
    }
    
    bool isMoveValid(int row, int col) {
        return row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ';
    }
    
    bool checkWin(char player) {
        for (int i = 0; i < 3; i++) {
            if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
                (board[0][i] == player && board[1][i] == player && board[2][i] == player))
                return true;
        }
        
        if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
            (board[0][2] == player && board[1][1] == player && board[2][0] == player))
            return true;
        
        return false;
    }
    
    bool isBoardFull() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ')
                    return false;
        return true;
    }
    
    void easyAIMove() {
        int row, col;
        do {
            row = rand() % 3;
            col = rand() % 3;
        } while (!isMoveValid(row, col));
        
        board[row][col] = 'O';
        cout << "  Computer plays at position " << (row * 3 + col + 1) << "\n";
    }
    
    int minimax(bool isMaximizing) {
        if (checkWin('O')) return 10;
        if (checkWin('X')) return -10;
        if (isBoardFull()) return 0;
        
        if (isMaximizing) {
            int bestScore = -1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'O';
                        int score = minimax(false);
                        board[i][j] = ' ';
                        bestScore = max(score, bestScore);
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = 1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'X';
                        int score = minimax(true);
                        board[i][j] = ' ';
                        bestScore = min(score, bestScore);
                    }
                }
            }
            return bestScore;
        }
    }
    
    void hardAIMove() {
        int bestScore = -1000;
        int bestRow = -1, bestCol = -1;
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    int score = minimax(false);
                    board[i][j] = ' ';
                    
                    if (score > bestScore) {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }
        
        if (bestRow != -1 && bestCol != -1) {
            board[bestRow][bestCol] = 'O';
            cout << "  Computer plays at position " << (bestRow * 3 + bestCol + 1) << "\n";
        }
    }
    
    void playerMove() {
        int position;
        bool validMove = false;
        
        while (!validMove) {
            cout << "  Player " << currentPlayer << ", enter position (1-9): ";
            
            if (!(cin >> position)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  [X] Invalid input! Please enter a number.\n";
                continue;
            }
            
            if (position < 1 || position > 9) {
                cout << "  [X] Invalid position! Choose 1-9.\n";
                continue;
            }
            
            int row = (position - 1) / 3;
            int col = (position - 1) % 3;
            
            if (isMoveValid(row, col)) {
                board[row][col] = currentPlayer;
                validMove = true;
            } else {
                cout << "  [X] Position already taken! Choose another.\n";
            }
        }
    }
    
public:
    TicTacToe() {
        srand(static_cast<unsigned int>(time(0)));
        initBoard();
    }
    
    void selectGameMode() {
        displayTitle();
        cout << "  +-----------------------------------+\n";
        cout << "  |      SELECT GAME MODE:            |\n";
        cout << "  +-----------------------------------+\n";
        cout << "  |  1. vs Computer (Easy)            |\n";
        cout << "  |  2. vs Computer (Hard)            |\n";
        cout << "  |  3. Two Player Mode               |\n";
        cout << "  +-----------------------------------+\n\n";
        cout << "  Enter your choice (1-3): ";
        
        while (!(cin >> gameMode) || gameMode < 1 || gameMode > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [X] Invalid choice! Enter 1, 2, or 3: ";
        }
    }
    
    void play() {
        displayTitle();
        
        string modeText;
        if (gameMode == 1) modeText = "Easy Mode";
        else if (gameMode == 2) modeText = "Hard Mode";
        else modeText = "Two Player Mode";
        
        cout << "  Game Mode: " << modeText << "\n";
        cout << "  Player X starts!\n";
        
        while (true) {
            displayBoard();
            
            if (currentPlayer == 'X' || gameMode == 3) {
                playerMove();
            } else {
                if (gameMode == 1)
                    easyAIMove();
                else
                    hardAIMove();
            }
            
            if (checkWin(currentPlayer)) {
                displayTitle();
                displayBoard();
                cout << "  ================================\n";
                cout << "  |   *** Player " << currentPlayer << " WINS! ***     |\n";
                cout << "  ================================\n\n";
                break;
            }
            
            if (isBoardFull()) {
                displayTitle();
                displayBoard();
                cout << "  ================================\n";
                cout << "  |   *** It's a DRAW! ***       |\n";
                cout << "  ================================\n\n";
                break;
            }
            
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }
    
    bool playAgain() {
        char choice;
        cout << "  Play again? (y/n): ";
        cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            initBoard();
            return true;
        }
        return false;
    }
};

int main() {
    cout << "\n\n";
    cout << "  ======================================\n";
    cout << "  |                                    |\n";
    cout << "  |     WELCOME TO TIC-TAC-TOE!        |\n";
    cout << "  |                                    |\n";
    cout << "  ======================================\n\n";
    
    TicTacToe game;
    
    do {
        game.selectGameMode();
        game.play();
    } while (game.playAgain());
    
    cout << "\n  ======================================\n";
    cout << "  |   Thanks for playing! Goodbye!     |\n";
    cout << "  ======================================\n\n";
    
    return 0;
}