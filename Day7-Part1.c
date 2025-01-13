#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/queue.h>
#include <stdbool.h>
/*
 * main.c
 *
 *  Created on: Nov 22, 2022
 *      Author: aubaker
 */

char input[] = "Input-Day7.txt";
FILE *fileptr;

struct crab {
    int position;
    LIST_ENTRY(crab) nextCrab;
};

LIST_HEAD(entries, crab);
struct entries allCrabs;
int maxPos, minPos;


void parseFileInput() {
    int crabPos = 0;
    maxPos = 0, minPos = 0;
    LIST_INIT(&allCrabs);

    while (fscanf(fileptr, "%d,", &crabPos) != EOF) {
        if (LIST_EMPTY(&allCrabs)){
            maxPos = crabPos;
            minPos = crabPos;
        } else {
            if (crabPos > maxPos){
                maxPos = crabPos;
            } else if (crabPos < minPos){
                minPos = crabPos;
            }
        }
        struct crab *newCrab = malloc(sizeof(struct crab));
        newCrab->position = crabPos;
        LIST_INSERT_HEAD(&allCrabs, newCrab, nextCrab);
    }
}

int identifyBestPosition(){
    
    int bestCost = 0;
    for (int i = minPos; i <= maxPos; i++){
        int totalCost = 0;
        struct crab *tmp = malloc(sizeof(struct crab));
        LIST_FOREACH(tmp, &allCrabs, nextCrab){
            if (i > tmp->position){
                int numSteps = i - tmp->position;
                for (int j = 1; j <= numSteps; j++) {
                    totalCost += j;
                }
            } else {
                int numSteps = tmp->position - i;
                for (int j = 1; j <= numSteps; j++) {
                    totalCost += j;
                }
            }
        }
        if (i == minPos){
            bestCost = totalCost;
        } else {
            if (totalCost < bestCost){
                bestCost = totalCost;                
            }
        }
    }
    return bestCost;
}



int main(int argc, char *argv[]) {
    // open file
	if ((fileptr = fopen(input, "r")) == NULL) {
        printf("No file found");
        exit(1);
    }
    parseFileInput();
    int cost = identifyBestPosition();

    printf("The best cost is: %d\n", cost);
}
