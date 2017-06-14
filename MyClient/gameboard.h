#ifndef GAMEBOARD_H
#define GAMEBOARD_H

enum StoneType {
    NONE,BLACK, WHITE
};
template <int N>
class GameBoard
{
private:
    StoneType board[N][N];
    int size;
public:
    GameBoard();
    //Show all the usage of public things.
    //Function : putStone
    //Usage: put a stone at position (x, y), check x and y
    //Input:
    // int x, the x coordinate of position to put sone [range: 0 ~ N-1]
    // int y, the y coordinate of position to put sone [range: 0 ~ N-1]
    // StoneType stone, must be BLACK or WHITE. Tell the user whether you checked player's turn.
    //Output:
    // bool, true if put stone succeessful. false if failed.
    //Demo:
    // putStone(7, 7, BLACK); //put a black stone at coordinate (7,7)
    bool putStone(int x, int y, StoneType stone);

    //Function : removeStone
    //Usage: remove a stone at position (x, y), check ……
    //Input:
    // int x, the x coordinate of position to put sone [range: 0 ~ N-1]
    // int y, the y coordinate of position to put sone [range: 0 ~ N-1]
    //Output:
    // bool, true if remove stone succeessful. false if failed.
    //Demo:
    // removeStone(7, 7); //remove a black stone at coordinate (7,7)
    bool removeStone(int x, int y);

    //Function : checkStone
    //Usage: check a stone's type at position (x, y), check x and y
    //Input:
    // int x, the x coordinate of position to put sone [range: 0 ~ N-1]
    // int y, the y coordinate of position to put sone [range: 0 ~ N-1]
    //Output:
    // StoneType, return the type at position (x, y)
    //Demo:
    // checkStone(7, 7); //check the type of position (7,7)
    StoneType checkStone(int x, int y) const;
    static int boardSize() ;
};

typedef GameBoard<19> GoGameBoard;
typedef GameBoard<15> FiveGameBoard;

template <int N>
GameBoard<N>::GameBoard()
{
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            board[i][j]=NONE;
        }
    }
}
template <int N>
bool GameBoard<N>::putStone(int x, int y, StoneType stone){
    if(x<N&&y<N&&stone!=NONE){
        if(checkStone(x,y)!=BLACK && checkStone(x,y)!=WHITE){
            board[x][y]=stone;
            return true;
        }
    }
    return false;
}

template <int N>
bool GameBoard<N>::removeStone(int x, int y){
    if(x<N&&y<N){
        board[x][y]=NONE;
        return true;
    }
    return false;
}

template <int N>
StoneType GameBoard<N>::checkStone(int x, int y) const{
    return board[x][y];
}

template <int N>
int GameBoard<N>::boardSize(){
    return N;
}

#endif // GAMEBOARD_H
