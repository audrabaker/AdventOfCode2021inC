#include <stdio.h>
#include <stdlib.h>

/*
 * main.c
 *
 *  Created on: Nov 7, 2022
 *      Author: aubaker
 */
// note to self- the values in the loops are 
// expecting the input value of 12- not the
// test input of 5. Adjust accordingly.
char input[] = "Input-Day3.txt";

int main(int argc, char *argv[]) {

    FILE *fileptr;
	if ((fileptr = fopen(input, "r")) == NULL) {
        printf("No file found");
        exit(1);
    }
    int total = 0;
	char value[13] = "";
	int bit[12] = {0};

	while (fscanf(fileptr, "%s\n", &value) != EOF) {
        //count the frequency of the 1's
        for (int i = 0; i < 12; i++){
            bit[i] += value[i] - '0';
        }
        total++;
    }

    int gammarate = 0;
    int epsilonrate = 0;

    //calculate the epsilon and gamma values
    for(int j = 0; j < 12; j++) {
        //1's are more common
        if (bit[j] > total - bit[j]) {
            gammarate += 1<<(11-j);
        } else {
            epsilonrate += 1<<(11-j);
        }
    }

    printf("%d  %d  power consumption is: %d\n", epsilonrate, gammarate, epsilonrate * gammarate);
	fclose(fileptr);
	return 0;
}