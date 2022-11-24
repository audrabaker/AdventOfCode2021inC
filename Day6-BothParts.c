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

char input[] = "Input-Day6.txt";
FILE *fileptr;

#define DAYS 256

unsigned long long int size, currentSize;

struct catalogOfFish {
    int startingTimer;
    unsigned long long int totalsize;
    LIST_ENTRY(catalogOfFish) nextStartingTimer;
};

LIST_HEAD(fishes, catalogOfFish);
struct fishes fishes;

void newFish(int daySpawned){

    if (daySpawned + 9 <= DAYS){
        daySpawned += 9;
        for (int i = daySpawned; i <= DAYS; i += 7){
            newFish(i);
        }
    }
    currentSize++;
}

void parseFileInput() {
    LIST_INIT(&fishes);
    size = 0;
    currentSize = 0;
    int fish = 0;
    bool found;
    while (fscanf(fileptr, "%d,", &fish) != EOF) {
        printf("current fish number: %d\n", fish);
        found = false;
        currentSize = 0;
        if (!LIST_EMPTY(&fishes)){
            //check if we have already processed this starting number.
            struct catalogOfFish *tmp = malloc(sizeof(struct catalogOfFish));
            LIST_FOREACH(tmp, &fishes, nextStartingTimer){
                printf("im checking the list.\n");
                if (fish == tmp->startingTimer){
                    size += tmp->totalsize;
                    found = true;
                    printf("i found a match in the list.\n");
                    printf("total size is: %llu  current size is: %llu\n", size, currentSize);
                    break;
                }
            }
        }

        if (found == true){
            continue;
            
        }
        // Account for self
        currentSize++;
        for (int i = fish + 1; i <= DAYS; i += 7) {
            // Create a new fish every 7 days.
            newFish(i);
        }
        struct catalogOfFish *newFish = malloc(sizeof(struct catalogOfFish));
        newFish->startingTimer = fish;
        newFish->totalsize = currentSize;
        LIST_INSERT_HEAD(&fishes, newFish, nextStartingTimer);

        size += currentSize;
        printf("total size is: %llu  current size is: %llu\n", size, currentSize);
    }
    printf("\nTotal number of fish after %llu days is: %llu.\n\n", DAYS, size);
}

int main(int argc, char *argv[]) {
    // open file
	if ((fileptr = fopen(input, "r")) == NULL) {
        printf("No file found");
        exit(1);
    }
    parseFileInput();
}
