#include <stdio.h>
#include <stdbool.h>
/*
 * main.c
 *
 *  Created on: Nov 7, 2022
 *      Author: aubaker
 */

char input[] = "input.txt";

int main(int argc, char *argv[]) {

	FILE *fileptr = fopen(input, "r");
	
	int firstnum, secondnum, thirdnum, fourthnum, increased = 0;
	int firstpass = 3;
	
	while (fscanf(fileptr, "%d\n", &fourthnum) != EOF) {
	  if (firstpass != 0) {
        firstpass--;
		continue;
	  } else if (firstnum + secondnum + thirdnum < secondnum + thirdnum + fourthnum) {
 		increased++;
	  }
	  firstnum = secondnum;
	  secondnum = thirdnum;
	  thirdnum = fourthnum;
    }
	printf("%d\n", increased);
	fclose(fileptr);
	return 0;
}
