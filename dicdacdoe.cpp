#include <iostream>
#include <vector>

using namespace std;

//Function to print the Tic-Tac-Toe board following every update. 
void printBoard(const vector<char>& board) {
    for (int i = 0; i < 9; i++) {
        cout << board[i] << " ";
        if ((i + 1) % 3 == 0) cout << endl;
    }
}

//Function to check if the game is over

bool isGameOver(const vector<char>& board) {
    //Check rows, columns, and diagonals for a win
    //if there are 3 of the same chars in either rows, cols, or diagonals, the function returns true and the game ends. It also returns false if there is a draw.
    for (int i = 0; i < 3; i++) {
        if (board[i] != '-' && board[i] == board[i + 3] && board[i] == board[i + 6]) return true;
        if (board[3 * i] != '-' && board[3 * i] == board[3 * i + 1] && board[3 * i] == board[3 * i + 2]) return true;
    }

    if (board[0] != '-' && board[0] == board[4] && board[0] == board[8]) return true;
    if (board[2] != '-' && board[2] == board[4] && board[2] == board[6]) return true;
    
    //Check for a draw
    for (char c : board) {
        if (c == '-') return false;
    }

    return true;
}

//Function to evaluate the board state for the minimax algorithm. returns 10 for a player win, -10 for a computer win.
int evaluateBoard(const vector<char>& board) {
    //Check rows, columns, and diagonals for wins

    for (int i = 0; i < 3; i++) {
        if (board[i] == board[i + 3] && board[i] == board[i + 6]) { //CHECKS COLUMNS FOR EQUAL CHARS
            if (board[i] == 'X') return 10;
            else if (board[i] == 'O') return -10; 
        }

        if (board[3 * i] == board[3 * i + 1] && board[3 * i] == board[3 * i + 2]) { //CHECKS HORIZONTALS FOR EQUAL CHARS    
            if (board[3 * i] == 'X') return 10;
            else if (board[3 * i] == 'O') return -10;
        }
    }
    //MANUAL CHECK FOR DIAGONALS FOR A 3X3 TICTACTOE BOARD

    if (board[0] == board[4] && board[0] == board[8]) { 
        if (board[0] == 'X') return 10; //top left to bottom right
        else if (board[0] == 'O') return -10;
    }
    if (board[2] == board[4] && board[2] == board[6]) { //top right to bottom left
        if (board[2] == 'X') return 10;
        else if (board[2] == 'O') return -10;
    }
    
    return 0;
}

//Minimax algorithm with alpha-beta pruning
int minimax(vector<char>& board, bool isMaximizer, int alpha, int beta) {
    if (isGameOver(board)) return evaluateBoard(board);

    if (isMaximizer) { //evaluates the maximizer (alpha value)
        int maxEval = -10000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == '-') {
                board[i] = 'X';
                int eval = minimax(board, false, alpha, beta); //RECURSIVE ITERATION. key aspect of minimax algorithm, basically checks all of the moves of tictactoe beforehand and evaluates the best one depending on the value of maxEval and minEval.
                maxEval = max(maxEval, eval);
                
                board[i] = '-';
                
                alpha = max(alpha, eval);
                if (beta <= alpha) break; //the prune. If the beta move ever becomes greater than the alpha move, then the move need not be considered. This is true for every branch of minimax algo.
            }
        }
        return maxEval; //will return following minimax. Computer will move depending on highest value of maxEval. 
    } 
    else { //evaluates the minimizer (beta value)
        int minEval = 10000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == '-') {
                board[i] = 'O';
                int eval = minimax(board, true, alpha, beta); //RECURSIVE ITERATION. 
                minEval = min(minEval, eval);
                
                board[i] = '-';

                beta = min(beta, eval); //beta prune function.
                if (beta <= alpha) break;
            }
        }
        return minEval; //will return following minimax. Computer will move depending on lowest value of minEval. 
    }
}

//Find the best move using the minimax algorithm
int findBestMove(vector<char>& board) {
    int bestMove = -1;
    int bestEval = -1000;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '-') {
            board[i] = 'X';
            int eval = minimax(board, false, -1000, 1000); //essentially simulates all moves for the computer in the future, calculates, then creates/prunes branches.
            board[i] = '-';

            if (eval > bestEval) {
                bestEval = eval;
                bestMove = i;
            } //gets max eval value. 
        }
    }
    return bestMove;
}

int main() {
    vector<char> board = { '-', '-', '-', '-', '-', '-', '-', '-', '-' };
    int currentPlayer = 0; //0 for player, 1 for computer

    cout << "Tic-Tac-Toe - Player vs Computer" << endl;

    while (!isGameOver(board)) {
        printBoard(board);

        if (currentPlayer == 0) {
            string playerMove;
            cout << "Player's turn. Enter move (0-8): ";
            cin >> playerMove;
            if(playerMove == "quit") return -1; //helps with testing.

            if (stoi(playerMove) < 0 || stoi(playerMove) > 8 || board[stoi(playerMove)] != '-') {
                cout << "Invalid move. Try again." << endl;
                continue;
            }

            board[stoi(playerMove)] = 'O';
        } 
        else {
            cout << "Computer's turn..." << endl;
            int computerMove = findBestMove(board);
            board[computerMove] = 'X';
        }

        currentPlayer = 1 - currentPlayer; //Switch players. 
    }

    printBoard(board);

    int result = evaluateBoard(board);

    if (result == 10) cout << "Computer wins!" << endl;
    else if (result == -10) cout << "Player wins!" << endl;
    else cout << "It's a draw!" << endl;

    return 0;
}