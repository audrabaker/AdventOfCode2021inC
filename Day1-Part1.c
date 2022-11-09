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
	
	int firstnum, secondnum, increased = 0;
	bool firstpass = true;
	
	while (fscanf(fileptr, "%d\n", &secondnum) != EOF) {
	  if (firstpass) {
		firstnum = secondnum;
		firstpass = false;
		continue;;
	  } else if (firstnum < secondnum) {
 		increased++;
	  }
	  firstnum = secondnum;
    }
	printf("%d\n", increased);
	fclose(fileptr);
	return 0;
}
