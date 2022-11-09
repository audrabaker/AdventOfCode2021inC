#include <stdio.h>
#include <string.h>

/*
 * main.c
 *
 *  Created on: Nov 7, 2022
 *      Author: aubaker
 */

char input[] = "Input-Day2.txt";

int main(int argc, char *argv[]) {

    //TODO add an exception handler here
	FILE *fileptr = fopen(input, "r");
    
	char direction[8];
	int amount, depth, horiz;
    amount = depth = horiz = 0;
	
	while (fscanf(fileptr, "%s %d\n", &direction, &amount) != EOF) {
        if (strcmp(direction, "forward") == 0){
            horiz += amount;
        } else if (strcmp(direction, "down") == 0) {
            depth += amount;
        } else {
            depth -= amount;
        }
    }
	printf("%d  %d  %d\n", horiz, depth, horiz * depth);
	fclose(fileptr);
	return 0;
}
