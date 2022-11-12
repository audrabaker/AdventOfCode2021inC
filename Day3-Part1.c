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
LIST_HEAD(firstpasslisthead, entry) firstpass;
LIST_HEAD(Oxlisthead, entry) OxRate;
LIST_HEAD(COlisthead, entry) COScrub;

int main(int argc, char *argv[]) {

    /* Open the file 
    */
	if ((fileptr = fopen(input, "r")) == NULL) {
        printf("No file found");
        exit(1);
    }

    /* Process the file input and create a list (firstpass)
     * of strings. While creating the list also count the 
     * number of 1's seen in the high order bit.  
     * The last variable is used to maintain the order of the
     * entries encountered- otherwise the list would be backwards.
    */
    int total = 0, numberOfOnes = 0;
	char value[MAXSIZE + 1] = "";
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
        total++;
    }

   
    /* We have our initial list (firstpass), now lets separate out 
     * the entries based on the most common bit seen (0's or 1's). 
     * Entries with the most common high order bit will be put 
     * in the OxRate list with the other entries being put in the 
     * COScrub list.
    */
    rewind(fileptr);
    LIST_INIT(&OxRate);
    LIST_INIT(&COScrub);
    int Oxsize = 0, COsize = 0, OxNumOfOnes = 0; 

    while (LIST_EMPTY(&firstpass) == 0) {
        //1's are more common
        struct entry *tmp = malloc(sizeof(struct entry));
        tmp = LIST_FIRST(&firstpass);
        LIST_REMOVE(tmp, entries);
        if (numberOfOnes >= total - numberOfOnes) {
            if (tmp->listelement[0] == '1') {
                LIST_INSERT_HEAD(&OxRate, tmp, entries);
                if (tmp->listelement[1] == '1') {
                    OxNumOfOnes++;
                }
                Oxsize++;
            } else {
                LIST_INSERT_HEAD(&COScrub, tmp, entries);
                COsize++;
            }
        } else {
            if (tmp->listelement[0] == '1') {
                LIST_INSERT_HEAD(&COScrub, tmp, entries);
                COsize++;
            } else {
                LIST_INSERT_HEAD(&OxRate, tmp, entries);
                if (tmp->listelement[1] == '1') {
                    OxNumOfOnes++;
                }
                Oxsize++;
            }
        }
    }



    /* Our data has been parsed into two lists according to
     * the most and least common high order bits. So now
     * start processing the OxRate list. 
    */
    
    int OxTotal = 0, OxNumOfOnesSecPass = 0;

    for (int i = 1; i < MAXSIZE - 1; i++) {
        struct entry *next = LIST_FIRST(&OxRate);
        OxTotal = Oxsize;
        do {
            if (Oxsize == 1) {
                break;
            }
            //1's are more common, so remove the 0s
            if (OxNumOfOnes >= OxTotal - OxNumOfOnes) {
                if (next->listelement[i] == '0') {
                    LIST_REMOVE(next, entries);
                    Oxsize--;
                } else if (next->listelement[i+1] == '1') {
                    OxNumOfOnesSecPass++;
                }
            } else {
                // 0's are more commmon so remove the 1s
                if (next->listelement[i] == '1') {
                    LIST_REMOVE(next, entries);
                    Oxsize--;
                } else if (next->listelement[i+1] == '1') {
                    OxNumOfOnesSecPass++;
                }     
            }
        } while(next = LIST_NEXT(next, entries));
        OxNumOfOnes = OxNumOfOnesSecPass;
        OxNumOfOnesSecPass = 0;
    }



    /* We've processed the OxRate list, so now let's 
     * process the CoScrub list.
    */
    int COTotal = 0, COOfOnesSecPass = 0, CONumofOnes = OxNumOfOnes;

    for (int i = 1; i < MAXSIZE - 1; i++) {
        struct entry *next = LIST_FIRST(&COScrub);
        COTotal = COsize;
        do {
            if (COsize == 1) {
                break;
            }

            //1's are more common, so remove the 1s
            if (CONumofOnes >= COTotal - CONumofOnes) {
                // if we have a 1, remove it
                if (next->listelement[i] == '1') {
                    LIST_REMOVE(next, entries);
                    COsize--;
                // Since we did not remove the list element,
                // check to see if the next bit has a 1.
                } else if (next->listelement[i+1] == '1') {
                    COOfOnesSecPass++;
                }
            } else {
                // 0's are more commmon so remove the 0s
                if (next->listelement[i] == '0') {
                    LIST_REMOVE(next, entries);
                    COsize--;
                } else if (next->listelement[i+1] == '1') {
                    COOfOnesSecPass++;
                }     
            }
        } while(next = LIST_NEXT(next, entries));

        struct entry *tmp = malloc(sizeof(struct entry));
        printf("size is %d\n", COsize);
        LIST_FOREACH(tmp, &COScrub, entries) {
            printf("%s\n", tmp->listelement);
        }
        printf("\n\n\n");
        CONumofOnes = COOfOnesSecPass;
        COOfOnesSecPass = 0;
    }

    // Now we have both our values, convert the value from a
    // string representing a binary value to a decimal number.

    int CONumber = 0, OxRating = 0;
    struct entry *tmp = malloc(sizeof(struct entry));
    tmp = LIST_FIRST(&OxRate);

    for (int i = 0; i < MAXSIZE; i++) {
        if (tmp->listelement[i] == '1') {
            OxRating += 1 << MAXSIZE - i - 1;
        }
    }
    printf("Oxrate is %s, which is %d\n", tmp->listelement, OxRating);
    
    tmp = LIST_FIRST(&COScrub);
    for (int i = 0; i < MAXSIZE; i++) {
        if (tmp->listelement[i] == '1') {
            CONumber += 1 << MAXSIZE - i - 1;
        }
    }
    printf("COscrub is %s, which is %d\n", tmp->listelement, CONumber);

    printf("lifesupport rating: %d\n", OxRating * CONumber);

	fclose(fileptr);
	return 0;
}