#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/queue.h>
#include <stdbool.h>
/*
 * main.c
 *
 *  Created on: Nov 7, 2022
 *      Author: aubaker
 */

char input[] = "Input-Day5.txt";
FILE *fileptr;
int row = 1;
int column = 1;

int *vents;

struct point {
    int Xcoord; 
    int Ycoord;
};

// Grow the size of the 2D array by an order of 10 based on 
// the X and Y coordinates.
void growVents( struct point *firstPT, struct point *secPT) {
    if (firstPT->Xcoord > column || secPT->Xcoord > column) {
        //figure out how much larger the X coordinates need to be:
        //    if the first xcoord is greater than secPT->Xcoord, set row
        //    to first->Xcoord. Otherwise secPT->Xcoord is larger, so set
        //    row to secPT->Xcoord.
        int newColumn = column;
        while (newColumn < firstPT->Xcoord || newColumn < secPT->Xcoord) {
            newColumn *= 10;
        }
        int *newVents = malloc((row * newColumn) * sizeof(int));
        for (int i = 0; i < row; i++){
            for (int j = 0; j < column; j++) {
                newVents[i * column + j] = vents[i * column + j];
            }
        }
        for (int i = 0; i < row; i++){
            for (int j = 0; j < newColumn; j++) {
                newVents[i * newColumn + j] = 0;
            }   
        }
        vents = newVents;
        column = newColumn;
    }

    if (firstPT->Ycoord > row || secPT->Ycoord > row) {
        //figure out how much larger the X coordinates need to be:
        //    if the first xcoord is greater than secPT->Xcoord, set row
        //    to first->Xcoord. Otherwise secPT->Xcoord is larger, so set
        //    row to secPT->Xcoord.
        int newRow = row;
        while (newRow < firstPT->Ycoord || newRow < secPT->Ycoord) {
            newRow *= 10;
        }
        int *newVents = malloc((newRow * column) * sizeof(int));
        for (int i = 0; i < row; i++){
            for (int j = 0; j < column; j++) {
                newVents[i * column + j] = vents[i * column + j];
            }
        }
        for (int i = row; i < newRow; i++){
            for (int j = 0; j < column; j++) {
                newVents[i * column + j] = 0;
            }   
        }
        vents = newVents;
        row = newRow;
    }
}

void parseFileInput() {
    struct point *firstPT = malloc(sizeof(struct point));
    struct point *secPT = malloc(sizeof(struct point));

    while (fscanf(fileptr, "%d,%d %*c%*c %d,%d", &firstPT->Xcoord, &firstPT->Ycoord, &secPT->Xcoord, &secPT->Ycoord) != EOF) {
        growVents(firstPT, secPT);

        
        
       // add the new vector to the array
       // we are a horiz or vert line.
       if (firstPT->Xcoord == secPT->Xcoord) {
            if (firstPT->Ycoord - secPT->Ycoord < 0) {
                for (int i = firstPT->Ycoord; i <= secPT->Ycoord; i++) {
                    int j = firstPT->Xcoord;
                    vents[i * column + j]++;
                }
            } else {
                for (int i = secPT->Ycoord; i <= firstPT->Ycoord; i++) {
                    int j = firstPT->Xcoord;
                    vents[i * column + j]++;
                }
            }
       } else if (firstPT->Ycoord == secPT->Ycoord) {
            if (firstPT->Xcoord - secPT->Xcoord < 0) {
                for (int j = firstPT->Xcoord; j <= secPT->Xcoord; j++) {
                    int i = firstPT->Ycoord;
                    vents[i * column + j]++;
                }
            } else {
                for (int j = secPT->Xcoord; j <= firstPT->Xcoord; j++) {
                    int i = firstPT->Ycoord;
                    vents[i * column + j]++;
                }
            } 
        // line must be diagonal.
        } else if (firstPT->Ycoord - secPT->Ycoord < 0 && firstPT->Xcoord - secPT->Xcoord < 0) {
                int j = firstPT->Xcoord;
                for (int i = firstPT->Ycoord; i <= secPT->Ycoord; i++) {                
                    vents[i * column + j]++;
                    j++;               
                }
        } else if (firstPT->Ycoord - secPT->Ycoord > 0 && firstPT->Xcoord - secPT->Xcoord > 0) {
                int j = secPT->Xcoord;
                for (int i = secPT->Ycoord; i <= firstPT->Ycoord; i++) {                
                    vents[i * column + j]++;
                    j++;               
                }
        } else if (firstPT->Ycoord - secPT->Ycoord < 0 && firstPT->Xcoord - secPT->Xcoord > 0) {
                int j = firstPT->Xcoord;
                for (int i = firstPT->Ycoord; i <= secPT->Ycoord; i++) {                
                    vents[i * column + j]++;
                    j--;               
                }
        } else if (firstPT->Ycoord - secPT->Ycoord > 0 && firstPT->Xcoord - secPT->Xcoord < 0) {
                int j = secPT->Xcoord;
                for (int i = secPT->Ycoord; i <= firstPT->Ycoord; i++) {                
                    vents[i * column + j]++;
                    j--;               
                }
        }

    }   
        



    // Draw the vent diagram and also calculate how many times the vents overlap more than 2x.
    int numOverlap = 0;
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            //printf("%d ", vents[i * column + j]);
            if (vents[i * column + j] > 1) {
                numOverlap++;
            }
        }
        //printf("\n");
    }
    printf("Number of Overlaps is: %d\n", numOverlap);

}


int main(int argc, char *argv[]) {
    vents = malloc((row *column) * sizeof(int));
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            vents[i * column + j] = 0;
        }
    }
    // open file
	if ((fileptr = fopen(input, "r")) == NULL) {
        printf("No file found");
        exit(1);
    }

    // parse the drawn numbers and the boards.
    parseFileInput();
    


    fclose(fileptr);
	return 0;
}