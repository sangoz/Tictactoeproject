#include <iostream>
#include <vector>
#include <random>
#include <windows.h>

using namespace std;

class Board {
private:
    char board[3][3] ={{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};
public:
    void printBoard(){
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if(j == 1){
                    if(board[i][j] == 'X'){
                        cout << " | ";
                        SET_COLOR(4);
                        cout << board[i][j];
                        SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        cout << " | ";
                    }
                    else if(board[i][j] == 'O'){
                        cout << " | ";
                        SET_COLOR(1);
                        cout << board[i][j];
                        SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        cout << " | ";
                    }
                    else{
                        cout << " | ";
                        cout << board[i][j];
                        cout << " | ";
                    }
                }
                else if(j == 0){
                    if(board[i][j] == 'X'){
                        SET_COLOR(4);
                        cout << " " << board[i][j];
                        SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    }
                    else if(board[i][j] == 'O'){
                        SET_COLOR(1);
                        cout << " " << board[i][j];
                        SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    }
                    else{
                        cout << " " << board[i][j];
                    }
                } 
                else{
                    if(board[i][j] == 'X'){
                        SET_COLOR(4);
                        cout << board[i][j];
                        SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    }
                    else if(board[i][j] == 'O'){
                        SET_COLOR(1);
                        cout << board[i][j];
                        SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    }
                    else{
                        cout << board[i][j];
                    }
                }    
            }
            cout << endl;
            if(i != 2)
                cout << "---|---|---" << endl;
        }
    }
    bool isWin(char player){
        for(int i = 0; i < 3; i++){
            if(board[i][0] == player && board[i][1] == player && board[i][2] == player) 
                return true;
            if(board[0][i] == player && board[1][i] == player && board[2][i] == player) 
                return true;
        }
        if(board[0][0] == player && board[1][1] == player && board[2][2] == player) 
            return true;
        if(board[0][2] == player && board[1][1] == player && board[2][0] == player) 
            return true;
        return false;
    }
    bool isDraw(){
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') return false;
            }
        }
        return true;
    }
    bool isValidMove(int move){
        if (move < 1 || move > 9) return false;
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        return (board[row][col] != 'X' && board[row][col] != 'O');
    }
    void updateBoard(int move, char player){
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        board[row][col] = player;
    }
    void SET_COLOR(int color){
        WORD wColor;
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
            wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
            SetConsoleTextAttribute(hStdOut, wColor);
        }
    }
};

class Player {
public:
    virtual int getMove() = 0;
};
class HumanPlayer : public Player{
public:
    int getMove(){
        int move;
        cout << "Enter your move (1-9): ";
        cin >> move;
        return move;
    }
};

class ComputerPlayer : public Player{
public:
    int getMove(){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 9);
        int move = dis(gen);
        cout << "Computer move: " << move << endl;
        return move;
    }
};

class Game{
private:
    Board board;
    Player* players[2];
public:
    Game(Player* player1, Player* player2){
        players[0] = player1;
        players[1] = player2;
    }
    void start(){
        char currentPlayer = 'X';
        while(!board.isWin(currentPlayer) || !board.isDraw()){
            board.printBoard();
            int move = players[currentPlayer == 'X' ? 0 : 1]->getMove();
            if(!board.isValidMove(move)){
                cout << "Invalid move. Please try again." << endl;
                continue;
            }
            board.updateBoard(move, currentPlayer);
            if (board.isWin(currentPlayer)){
                board.printBoard();
                cout << "Player " << currentPlayer << " wins!" << endl;
                break;
            }
            if (board.isDraw()){
                board.printBoard();
                cout << "It's a draw!" << endl;
                break;
            }
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }
    static int main(){
        HumanPlayer human;
        ComputerPlayer computer;
        string CHOOSE_MODE = "Enter mode(pvp/pvm/mvp/mvm) or 'exit' to quit: ";
        string mode;
        cout << CHOOSE_MODE;
        while(cin >> mode && mode != "exit"){
            if(mode == "pvp"){
                Game game(&human, &human);
                game.start();
            }
            else if(mode == "pvm"){
                Game game(&human, &computer);
                game.start();
            }
            else if(mode == "mvp"){
                Game game(&computer, &human);
                game.start();
            }
            else if(mode == "mvm"){
                Game game(&computer, &computer);
                game.start();
            }
            else{
                cout << "Invalid mode. " << endl;
            }
            cout << CHOOSE_MODE;
        }
        return 0;
    }
};

int main(){
    return Game::main();
}

