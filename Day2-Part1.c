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

    FILE *fileptr;
	if ((fileptr = fopen(input, "r")) == NULL) {
        printf("No file found");
        exit(1);
    }

	char direction[8];
	int amount, depth, horiz, aim;
    amount = depth = horiz = aim = 0;
	
	while (fscanf(fileptr, "%s %d\n", &direction, &amount) != EOF) {
        if (strcmp(direction, "forward") == 0){
            horiz += amount;
            depth += amount * aim;
        } else if (strcmp(direction, "down") == 0) {
            aim += amount;
        } else {
            aim -= amount;
        }
    }
	printf("%d  %d  %d\n", horiz, depth, horiz * depth);
	fclose(fileptr);
	return 0;
}
