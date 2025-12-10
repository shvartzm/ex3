#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

#define FLAG_FOUR 0
#define FLAG_THREE 1

int isColumnFull(char[][COLS], int, int); // check if collumn is full

int isBoardFull(char[][COLS], int, int); // check if board full

int isInBounds(int, int, int, int); // check if given spot is within bounds

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char[][COLS], int,int);

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char[][COLS], int, int, char);

int checkVictory(char[][COLS], int, int, int, int, char,int); // check if possible victory

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char[][COLS], int, int,char);

/* Computer*/
//int computerChoose(char[][COLS], int, int, char, char);
int computerChoose(char[][COLS], int, int,char);

void runConnectFour(char[][COLS], int, int, int, int); // run game  

void initBoard(char[][COLS], int, int); // fill board with EMPTY value 

void printBoard(char[][COLS], int, int); // print the board

int getPlayerType(int);

int checkUpDown(char[][COLS], int, int, int, int, char,int); // check possible 3/4 sequence up/down
int checkRightLeft(char[][COLS], int, int, int, int, char,int); // check possible 3/4 sequence right/left
int checkRightDiagonal(char[][COLS],int, int, int, int, char,int); // check possible 3/4 sequence on the right diagonal
int checkLeftDiagonal(char[][COLS], int, int, int , int, char,int);// check possible 3/4 sequence on the left diagonal


int main() {
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS); // emptying board
    printBoard(board, ROWS, COLS); 
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

void printBoard(char board[][COLS], int rows, int cols) {
    printf("\n");
    for (int r = 0; r < rows; r++) {
        printf("|");
        for (int c = 0; c < cols; c++) {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++) {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

int getPlayerType(int playerNumber) {
    char ch;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1) {
            printf("Input error. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H') return HUMAN;
        if (ch == 'c' || ch == 'C') return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n'); // clear rest of input
    }
}

void initBoard(char board[][COLS], int rows, int cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols ; j++){
            board[i][j] = EMPTY; // fill with EMPTY
        }
    }
}

void runConnectFour(char board[][COLS], int rows, int cols, int type1, int type2){
    int result = 0; //  1 win
    int move = type1;   // curr player type
    int currPlayer = 1;
    char currToken = TOKEN_P1;

    while(result == 0){
        if (isBoardFull(board,rows,cols)){
            printf("Board full and no winner. It's a tie!\n"); // check for tie
            break;
        }
        printf("Player %d (%c) turn.\n", currPlayer, currToken); // print curr turn
         // check how to procced
        result = (move == HUMAN)
             ? humanChoose(board, rows, cols,currToken) 
             : computerChoose(board, rows, cols, currToken);
        printBoard(board, ROWS, COLS);
        if (result){
            printf("Player %d (%c) wins!", currPlayer, currToken); // print win
            break;
        }
        currPlayer = currPlayer == 1 ? 2 : 1; // change player
        move = move == type1 ? type2 : type1;
        currToken = currToken == TOKEN_P1 ? TOKEN_P2 : TOKEN_P1; // change token
    }
}  

int isColumnFull(char board[][COLS], int rows, int collum){
    for (int i = rows - 1; i >= 0; i--){
        if (board[i][collum-1] == EMPTY){ // if found slot which is empty -> column not full
            return 0;
        }
    }
    return 1;
}

int isBoardFull(char board[][COLS], int rows, int cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (board[i][j] == EMPTY){ // if found slot which is empty - > board not full
                return 0;
            }
        }
    }
    return 1;
}

int isInBounds(int rows, int cols, int row, int col){
    return (row >= 0 && row < rows) && (col >= 0 && col < cols);
}


int makeMove(char board[][COLS], int rows, int collum, char token){
    for(int i = rows -1; i >= 0; i--){
        if (board[i][collum - 1] == EMPTY){ // go from last row, up, if empty, insert token
            board[i][collum - 1] = token;
            return i;
        }
    }
    return -1;
}

int checkUpDown(char board[][COLS], int rows, int cols, int row, int col, char token, int flagseq){
    int flagUp = 1; // make sure up wasn't enemy
    int flagDown = 1; // make sure down wasn't enemy
    int sumTokens = 1; // sum of tokens in a row

    for (int i = 1; i < (CONNECT_N - flagseq); i++){
        /* we go up and go down at the same time, if found enemy/empty above or below, we set flag to be 0,
        then we check if sum is 4- our flagseq, 0 for 4 sequence and 1 for 3 sequence */ 
        flagUp &= isInBounds(rows,cols, row - i, col) && (board[row - i][col] == token);
        flagDown &= isInBounds(rows, cols, row + i, col) && (board[row + i][col] == token);
        sumTokens += flagUp + flagDown;
        if (sumTokens >= (CONNECT_N-flagseq)) return 1;
    }
    return 0;
}


int checkRightLeft(char board[][COLS], int rows, int cols, int row, int col, char token, int flagseq){
    int flagRight = 1; // make sure right wasn't enemy
    int flagLeft = 1; // make sure left wasn't enemy
    int sumTokens = 1; // sum of tokens in a row

    for (int i = 1; i < (CONNECT_N - flagseq); i++){
        /* we go right and go left at the same time, if found enemy/empty right or left, we set flag to be 0,
        then we check if sum is 4- our flagseq, 0 for 4 sequence and 1 for 3 sequence */ 
        flagRight &= isInBounds(rows, cols, row, col + i) && (board[row][col + i] == token);
        flagLeft &= isInBounds(rows,cols, row, col - i) && (board[row][col - i] == token);
        sumTokens += flagRight + flagLeft;
        if (sumTokens >= (CONNECT_N-flagseq)) return 1;
    }
    return 0;
}

int checkRightDiagonal(char board[][COLS], int rows, int cols, int row, int col, char token, int flagseq){
    int flagRightUp = 1; // make sure right up wasn't enemy
    int flagLeftDown = 1; // make sure left down wasn't enemy
    int sumTokens = 1; 
    for (int i = 1; i < (CONNECT_N - flagseq); i++){
        /* we go up right and go down left at the same time, if found enemy/empty 
        up right or down left we set flag to 0 */
        flagRightUp &= isInBounds(rows, cols, row - i , col + i) && (board[row - i][col + i] == token);
        flagLeftDown &= isInBounds(rows,cols, row + i, col - i) && (board[row + i][col - i] == token);
        sumTokens += flagRightUp + flagLeftDown;
        if (sumTokens >= (CONNECT_N - flagseq)) return 1;
    }
    return 0;
}

int checkLeftDiagonal(char board[][COLS], int rows, int cols, int row, int col, char token, int flagseq){
    int flagLeftUp = 1; // make sure right up wasn't enemy
    int flagRightDown = 1; // make sure left down wasn't enemy
    int sumTokens = 1; 
    for (int i = 1; i < (CONNECT_N - flagseq); i++){
        /* we go up left and go down right at the same time, if found enemy/empty 
        up left or down right we set flag to 0 */
        flagLeftUp &= isInBounds(rows, cols, row - i, col - i) && (board[row - i][col - i] == token);
        flagRightDown &= isInBounds(rows,cols, row + i, col + i) && (board[row + i][col + i] == token);
        sumTokens += flagLeftUp + flagRightDown;
        if (sumTokens >= (CONNECT_N - flagseq)) return 1;
    }
    return 0;
}

int checkVictory(char board[][COLS], int rows, int cols, int row, int col, char token, int flagseq){
    // combine all the checks together, -1 for index fixing
    int checkUp = checkUpDown(board,rows,cols,row - 1,col - 1,token,flagseq);
    int checkRight = checkRightLeft(board,rows,cols,row - 1,col - 1,token,flagseq);
    int checkRightDia = checkRightDiagonal(board,rows,cols,row - 1,col - 1,token,flagseq);
    int checkLeftDia = checkLeftDiagonal(board,rows,cols,row - 1 ,col - 1,token,flagseq);
    return checkUp || checkRight || checkRightDia || checkLeftDia;
}

int humanChoose(char board[][COLS], int rows, int cols, char token){
    int chosencol;
    int chosenRow;
    printf("Enter column (1-%d): ", COLS);
    int isInteger = scanf("%d", &chosencol); // check for mis inputs
    while(! ((chosencol >= 1 && chosencol <= cols) && isInteger && !isColumnFull(board,rows,chosencol))){ 
        // loop until we get desired input
        if (!isInteger) {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n'); // clear input buffer
        }
        else if(!(chosencol >= 1 && chosencol <= cols)){
             printf("Invalid column. Choose between 1 and %d.\n", cols);
        }
        else{
            printf("Column %d is full. Choose another column.\n", chosencol);
        }
        printf("Enter column (1-%d): ", COLS);
        isInteger = scanf("%d", &chosencol);
    }
    chosenRow = makeMove(board,rows,chosencol,token); // if input valid make move
    return checkVictory(board,rows,cols,chosenRow + 1,chosencol,token,FLAG_FOUR); // check for case of winning move
}

int computerChoose(char board[][COLS], int rows, int cols, char currToken){
    int startingCol = (cols + 1) /2; // set starting index for prioriy actions
    int i =0;
    int multiplier = cols % 2 == 0 ? 1 : -1; // seperate for even and odd collumn number
    int currIndex = startingCol;
    int step = 1;
    char enemyToken = currToken == TOKEN_P1 ? TOKEN_P2 : TOKEN_P1;
    while (i < cols){
        // make win function
            if(!isColumnFull(board,rows,currIndex)){
                if(checkVictory(board,rows,cols,getFreeRow(board,rows,currIndex) + 1, currIndex, currToken,FLAG_FOUR)){
                    // check for victory in the possible row in the coloumn going with our priority
                    makeMove(board,rows,currIndex, currToken);
                    printf("Computer chose column %d\n", currIndex);
                    return 1;
                }
            }
            // we bassically want to always go one step farther in the opposite direction....
            currIndex = currIndex + (step * multiplier);
            step++;
            multiplier *= -1;
            i++;
    }
    //reset values
    i = 0;
    multiplier = cols % 2 == 0 ? 1 : -1;
    currIndex = startingCol;
    step = 1;

    
    while (i < cols){
        // block win function
        if(!isColumnFull(board,rows,currIndex)){
            if(checkVictory(board,rows,cols,getFreeRow(board,rows,currIndex)+ 1,currIndex,enemyToken,FLAG_FOUR)){
                // check if enemy can possibly win their next move
                makeMove(board,rows,currIndex, currToken);
                printf("Computer chose column %d\n", currIndex);
                return 0;
            }
        }
        // same as before
        currIndex = currIndex + (step * multiplier);
        step++;
        multiplier *= -1;
        i++;
    }
    //reset values
    i = 0;
    multiplier = cols % 2 == 0 ? 1 : -1;
    currIndex = startingCol;
    step = 1;
  
    while (i < cols){
        // take 3 sequence
        if(!isColumnFull(board,rows,currIndex)){
            if(checkVictory(board,rows,cols,getFreeRow(board,rows,currIndex)+ 1,currIndex,currToken,FLAG_THREE)){
                //check if possible to create 3 sequence move
                makeMove(board,rows,currIndex, currToken);
                printf("Computer chose column %d\n", currIndex);
                    return 0;
            }
        }
        // same as before
        currIndex = currIndex + (step * multiplier);
        step++;
        multiplier *= -1;
        i++;
    }
    
    //reset our values
    i = 0;
    multiplier = cols % 2 == 0 ? 1 : -1;
    currIndex = startingCol;
    step = 1;

    while (i < cols){
        // block three function
        if(!isColumnFull(board,rows,currIndex)){
            if(checkVictory(board,rows,cols,getFreeRow(board,rows,currIndex)+ 1,currIndex,enemyToken,FLAG_THREE)){
                //check if possible to block enemy's future 3 sequence
                makeMove(board,rows,currIndex, currToken);
                printf("Computer chose column %d\n", currIndex);
                    return 0;
            }
        }
        // same as before
        currIndex = currIndex + (step * multiplier);
        step++;
        multiplier *= -1;
        i++;
    }
    
    // reset our values
    i = 0;
    multiplier = cols % 2 == 0 ? 1 : -1;
    currIndex = startingCol;
    step = 1;

    while (i < cols){
        //  make move based on priorities
        if(!isColumnFull(board,rows,currIndex)){
                makeMove(board,rows,currIndex, currToken);
                printf("Computer chose column %d\n", currIndex);
                return 0;
        }
        // reset values
        currIndex = currIndex + (step * multiplier);
        step++;
        multiplier *= -1;
        i++;
    }
    return 0;

}

int getFreeRow(char board[][COLS], int rows, int collumn){
    for(int i = rows -1; i >= 0; i--){
        if (board[i][collumn - 1] == EMPTY){ // return row   if free
            return i;
        }
    }
    return -1;
}



