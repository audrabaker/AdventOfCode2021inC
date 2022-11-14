#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/queue.h>
#include <stdbool.h>

char input[] = "Test-Day4.txt";
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

void parseTheBoards() {

    struct board *newBoard = malloc(sizeof(struct board));
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
        // We are not the first board, but it is time for a new board
        } else if (row == 0) {
            struct board *tmp = malloc(sizeof(struct board));
            LIST_INSERT_AFTER(newBoard, tmp, nextBoard);
            newBoard = tmp;
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
}

void debugBoards() {
    struct board *tmp = malloc(sizeof(struct board));

    LIST_FOREACH(tmp, &boards, nextBoard) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++){
                printf("%2d ", tmp->grid[i][j]);
                if (j == 4) {
                    printf("\n");
                }
            }
        }
        printf("\n");
    }
}

void parseFileInput() {
    parseTheFirstLine();
    parseTheBoards();
    debugBoards();
}

void playBingo() {
    struct numberToDraw *currentNumber = malloc(sizeof(struct numberToDraw));   
    struct board *tmpBoard = malloc(sizeof(struct board));

    LIST_FOREACH(currentNumber, &numToDraw, nextNumber) {
        LIST_FOREACH(tmpBoard, &boards, nextBoard) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (tmpBoard->grid[i][j] == currentNumber->number) {
                        tmpBoard->called[i][j] = true;
                        printf("%d %s\n", currentNumber->number, tmpBoard->called[i][j] ? "true" : "false");
                    }
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
    playBingo();
    // after the first 5 numbers are drawn, check for a winner
    // check horiz
    // check vertical
    // check cris-cross
    // once a winner is found, calc score
    // add up unmarked numbers from winning board
    // multiply unmarked numbers sum with the last number drawn.

}