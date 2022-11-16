#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/queue.h>
#include <stdbool.h>

char input[] = "Input-Day4.txt";
FILE *fileptr;

struct numberToDraw {
    int number;
    LIST_ENTRY(numberToDraw) nextNumber;
};

struct board {
    int grid[5][5];
    bool called[5][5];
    LIST_ENTRY(board) nextBoard;
};

LIST_HEAD(entries, numberToDraw);
struct entries numToDraw;

LIST_HEAD(boards, board);
struct boards boards;
int totalNumOfBoards = 0;


// The first line has its own unique delimiters so we need to 
// parse it on its own. 
void parseTheFirstLine() {
    LIST_INIT(&numToDraw);

    int value = 0;
    char commaDelimiter;
    struct numberToDraw *last = malloc(sizeof(struct numberToDraw));
    last = LIST_FIRST(&numToDraw);
    // process the first line of input which is all the numbers to be
    // drawn for bingo. 
    while (fscanf(fileptr, "%d%c", &value, &commaDelimiter) != EOF) {
        if (LIST_EMPTY(&numToDraw)) {
            struct numberToDraw *numberToDraw = malloc(sizeof(struct numberToDraw));
            numberToDraw->number = value;
            LIST_INSERT_HEAD(&numToDraw, numberToDraw, nextNumber);
            last = numberToDraw;
        } else {
            struct numberToDraw *numberToDraw = malloc(sizeof(struct numberToDraw));
            numberToDraw->number = value;
            LIST_INSERT_AFTER(last, numberToDraw, nextNumber);
            last = numberToDraw;
        }
        if (commaDelimiter == '\n'){
            break;
        }
    }
}

// After the first line, we have the boards. So parse those into
// 5x5 grids.
void parseTheBoards() {

    struct board *newBoard = malloc(sizeof(struct board));
    //initialize the first board to have all the "called"
    // entries as false.
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            newBoard->called[i][j] = false;
        }
    }
    LIST_INIT(&boards);

    int row = 0, grid[5] = {0};
    
    while (fscanf(fileptr, "%d %d %d %d %d ", &grid[0], &grid[1], &grid[2], &grid[3], &grid[4]) != EOF) {
        // Are we the first board?
        if (LIST_EMPTY(&boards)) {
            LIST_INSERT_HEAD(&boards, newBoard, nextBoard);
            totalNumOfBoards++;
        // We are not the first board, but it is time for a new board
        } else if (row == 0) {
            struct board *tmp = malloc(sizeof(struct board));
            LIST_INSERT_AFTER(newBoard, tmp, nextBoard);
            newBoard = tmp;
            totalNumOfBoards++;
            // initialize all the called entries to false.
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    newBoard->called[i][j] = false;
                }
            }
        }
        // fill a line of the board's grid.
        for (int i = 0; i < 5; i++) {
            newBoard->grid[row][i] = grid[i];
        }
        // account for what row we are on.
        row++;
        // if it's time for a new board, reset the row counter.
        if (row == 5){
            row = 0;
        }
    }
    printf("totalNumer of Boards: %d\n", totalNumOfBoards);
}
// For debugging the boards if needed. 
void debugBoards() {
    struct board *tmp = malloc(sizeof(struct board));
    char ESC=27;
    LIST_FOREACH(tmp, &boards, nextBoard) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++){
                if (tmp->called[i][j]) {
                    printf("%c[1m",ESC);  /*- turn on bold */
                    printf("%2d ", tmp->grid[i][j]);
                    printf("%c[0m",ESC); /* turn off bold */
                } else {
                    printf("%2d ", tmp->grid[i][j]);
                }
                if (j == 4) {
                    printf("\n");
                }
            }
        }
        printf("\n");
    }
}

// The full flow for parsing the input file.
void parseFileInput() {
    parseTheFirstLine();
    parseTheBoards();
    //debugBoards();
}

// Once we have the winning board we calculate
// the score by adding the uncalled numbers on
// that board.
int calcUnmarkedNumbers(struct board *board) {
    int unmarkedSum = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (!board->called[i][j]){
                unmarkedSum += board->grid[i][j];
            }
        }
    }
    return unmarkedSum;
}

// Check for winners by looking at each board horizontally,
// vertically and then diagonally (criss-cross).
// Once a winning board is found, we remove the board from
// the list of boards.
int checkForWinner(struct board *tmpBoard) {
    // check horiz

        for (int row = 0; row < 5; row++){
            int amIaWinner = 0;
            for (int column = 0; column < 5; column++) {
                if (tmpBoard->called[row][column] == true){
                    amIaWinner++;
                }
            }
            if (amIaWinner == 5){
                if (totalNumOfBoards != 1) {
                    LIST_REMOVE(tmpBoard, nextBoard);
                    totalNumOfBoards--;
                    return 0;
                } else if (totalNumOfBoards == 1) {
                    return calcUnmarkedNumbers(tmpBoard);
                }
            }
        }
    

    // check vertical
   

        for (int column = 0; column < 5; column++){
            int amIaWinner = 0;
            for (int row = 0; row < 5; row++) {
                if (tmpBoard->called[row][column] == true){
                    amIaWinner++;
                } 
                if (amIaWinner == 5){
                    if (totalNumOfBoards != 1) {
                        LIST_REMOVE(tmpBoard, nextBoard);
                        totalNumOfBoards--;
                        return 0;
                    } else if (totalNumOfBoards == 1) {
                        return calcUnmarkedNumbers(tmpBoard);
                    }
                }
            }
        }
    

    /*
    // check cris-cross

        int amIaWinnerDiagDown = 0;
        int amIaWinnerDiagUp = 0;
        for (int i = 0; i < 5; i++){
            if (tmpBoard->called[i][i] == true){
                    amIaWinnerDiagDown++;
            }
            if (tmpBoard->called[i][5-i]) {
                    amIaWinnerDiagUp++;
            }   
            if (amIaWinnerDiagDown == 5 || amIaWinnerDiagUp == 5) {
                if (totalNumOfBoards != 1) {
                    LIST_REMOVE(tmpBoard, nextBoard);
                    totalNumOfBoards--;
                    return 0;
                } else if (totalNumOfBoards == 1) {
                    return calcUnmarkedNumbers(tmpBoard);
                }
            }
        }
        */
    
    return 0;
}


// Play Bingo by calling numbers (from the input), then checking
// the boards and marking the boards if the number is found on 
// the board. After 5 numbers are called, start checking for 
// winners. Once a winner is found the final score is 
// taken from the return of checkForWinner() - which is the 
// sum of the uncalled numbers from the winning board -multiplied
// by the final number called.
void playBingo() {
    struct numberToDraw *currentNumber = malloc(sizeof(struct numberToDraw));   
    struct board *tmpBoard = malloc(sizeof(struct board));
    int iteration = 0;

    LIST_FOREACH(currentNumber, &numToDraw, nextNumber) {
        iteration++;
        LIST_FOREACH(tmpBoard, &boards, nextBoard) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (tmpBoard->grid[i][j] == currentNumber->number) {
                        tmpBoard->called[i][j] = true;
                    }
                }
            }
        }
        // after the first 5 numbers are drawn, check for a winner
        if (iteration >= 5) {
            struct board *tmp = malloc(sizeof(struct board));
            LIST_FOREACH(tmp, &boards, nextBoard) {
                int winningSum = checkForWinner(tmp);
                if (winningSum != 0) {
                    printf("losing sum is: %d  Last number called is: %d\n", winningSum, currentNumber->number);
                    printf("total score for last board to win is: %d\n", winningSum * currentNumber->number);
                    return;
                }
            }
        }

    }

}



int main(int argc, char *argv[]) {

    // open file
	if ((fileptr = fopen(input, "r")) == NULL) {
        printf("No file found");
        exit(1);
    }

    // parse the drawn numbers and the boards.
    parseFileInput();
    
    // play bingo - mark drawn numbers on boards 
    // once a winner is found, remove the board from the list.
    // Once there is only one remaining board in the list,
    // add up unmarked numbers from that board
    // multiply unmarked numbers sum with the last number drawn.
    playBingo();

    fclose(fileptr);
	return 0;
}