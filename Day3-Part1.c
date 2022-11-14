#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/queue.h>

/*
 * main.c
 *
 *  Created on: Nov 7, 2022
 *      Author: aubaker
 */

// The size of the list elements in the 
// input files can either be 5 or 12
// bits in size- asjust according to 
// the input file.
char input[] = "Input-Day3.txt";

//(input == "Test-Day3.txt") ? 5 : 12
#define MAXSIZE 12
FILE *fileptr;

struct entry {
    char listelement[13];
    LIST_ENTRY(entry) entries;
};

LIST_HEAD(entries, entry);

struct entries firstpass, OxRate, COScrub;

// Sizes for the respective lists. 
// Also counters for the number of times 1 is seen for each list.
int totalNumOfEntries = 0, OxRatesize = 0, COScrubsize = 0, OxNumOfOnes = 0, CONumOfOnes = 0; 

/* Process the file input and create a list (firstpass)
 * of strings. While creating the list also count the 
 * number of 1's seen in the high order bit.  
 * The last variable is used to maintain the order of the
 * entries encountered- otherwise the list would be backwards.
*/
int processFileIntoList() {
	char value[MAXSIZE + 1] = "";

    int numberOfOnes = 0;
    LIST_INIT(&firstpass);
    struct entry *last;
    
    while (fscanf(fileptr, "%s\n", &value) != EOF) {
        if (LIST_EMPTY(&firstpass) != 0) {
            struct entry *newentry = malloc(sizeof(struct entry));
            strcpy(newentry->listelement, value);
            LIST_INSERT_HEAD(&firstpass, newentry, entries);
            last = newentry;
        } else {
            struct entry *newentry = malloc(sizeof(struct entry));
            strcpy(newentry->listelement, value);
            LIST_INSERT_AFTER(last, newentry, entries);
            last = newentry;
        }
        if (value[0] == '1') {
            numberOfOnes++;
        }
        totalNumOfEntries++;
    }
    return numberOfOnes;
}

void processFirstList(int numberOfOnes) {
    while (LIST_EMPTY(&firstpass) == 0) {
        //1's are more common
        struct entry *tmp = malloc(sizeof(struct entry));
        tmp = LIST_FIRST(&firstpass);
        LIST_REMOVE(tmp, entries);

        if (numberOfOnes >= totalNumOfEntries - numberOfOnes) {
            if (tmp->listelement[0] == '1') {
                LIST_INSERT_HEAD(&OxRate, tmp, entries);
                if (tmp->listelement[1] == '1') {
                    OxNumOfOnes++;
                }
                OxRatesize++;
            } else {
                LIST_INSERT_HEAD(&COScrub, tmp, entries);
                COScrubsize++;
                if (tmp->listelement[1] == '1') {
                    CONumOfOnes++;
                }
            }
        } else {
            if (tmp->listelement[0] == '1') {
                LIST_INSERT_HEAD(&COScrub, tmp, entries);
                COScrubsize++;
                if (tmp->listelement[1] == '1') {
                    CONumOfOnes++;
                }
            } else {
                LIST_INSERT_HEAD(&OxRate, tmp, entries);
                if (tmp->listelement[1] == '1') {
                    OxNumOfOnes++;
                }
                OxRatesize++;
            }
        }
    }
}

void processO2List() {
    int OxNumOfOnesSecPass = 0;
    for (int i = 1; i < MAXSIZE - 1; i++) {
        struct entry *next = LIST_FIRST(&OxRate);
        int total = OxRatesize;
        do {
            if (OxRatesize == 1) {
                break;
            }
            //1's are more common, so remove the 0s
            if (OxNumOfOnes >= total - OxNumOfOnes) {
                if (next->listelement[i] == '0') {
                    LIST_REMOVE(next, entries);
                    OxRatesize--;
                } else if (next->listelement[i+1] == '1') {
                    OxNumOfOnesSecPass++;
                }
            } else {
                // 0's are more commmon so remove the 1s
                if (next->listelement[i] == '1') {
                    LIST_REMOVE(next, entries);
                    OxRatesize--;
                } else if (next->listelement[i+1] == '1') {
                    OxNumOfOnesSecPass++;
                }     
            }
        } while(next = LIST_NEXT(next, entries));
        OxNumOfOnes = OxNumOfOnesSecPass;
        OxNumOfOnesSecPass = 0;
    }

}

void processCOList() {

    int COOfOnesSecPass = 0;
    
    for (int i = 1; i < MAXSIZE - 1; i++) {
        struct entry *next = LIST_FIRST(&COScrub);
        int total = COScrubsize;
        do {
            if (COScrubsize == 1) {
                break;
            }

            //1's are more common, so remove the 1s
            if (CONumOfOnes >= total - CONumOfOnes) {
                // if we have a 1, remove it
                if (next->listelement[i] == '1') {
                    LIST_REMOVE(next, entries);
                    COScrubsize--;
                // Since we did not remove the list element,
                // check to see if the next bit has a 1.
                } else if (next->listelement[i+1] == '1') {
                    COOfOnesSecPass++;
                }
            } else {
                // 0's are more commmon so remove the 0s
                if (next->listelement[i] == '0') {
                    LIST_REMOVE(next, entries);
                    COScrubsize--;
                } else if (next->listelement[i+1] == '1') {
                    COOfOnesSecPass++;
                }     
            }
        } while(next = LIST_NEXT(next, entries));
        CONumOfOnes = COOfOnesSecPass;
        COOfOnesSecPass = 0;
    }
}

int convertToBinary(struct entries *list) {
    int decimalVal = 0;
    struct entry *tmp = malloc(sizeof(struct entry));
    tmp = LIST_FIRST(list);

    for (int i = 0; i < MAXSIZE; i++) {
        if (tmp->listelement[i] == '1') {
            decimalVal += 1 << MAXSIZE - i - 1;
        }
    }
    return decimalVal;
}

int main(int argc, char *argv[]) {

    /* Open the file 
    */
	if ((fileptr = fopen(input, "r")) == NULL) {
        printf("No file found");
        exit(1);
    }

    int NumberOfOnes = processFileIntoList();
  
    /* We have our initial list (firstpass), now lets separate out 
     * the entries based on the most common bit seen (0's or 1's). 
     * Entries with the most common high order bit will be put 
     * in the OxRate list with the other entries being put in the 
     * COScrub list.
    */
    rewind(fileptr);
    LIST_INIT(&OxRate);
    LIST_INIT(&COScrub);
    processFirstList(NumberOfOnes);


    /* Our data has been parsed into two lists according to
     * the most and least common high order bits. So now
     * start processing the OxRate list. 
    */
    processO2List();

    /* We've processed the OxRate list, so now let's 
     * process the CoScrub list.
    */
    processCOList();

    printf("Lifesupport Rating: %d\n", convertToBinary(&OxRate) * convertToBinary(&COScrub));

	fclose(fileptr);
	return 0;
}