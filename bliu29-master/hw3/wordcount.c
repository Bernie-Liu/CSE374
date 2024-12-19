// Copyright 2024 Bernie Liu
// Author: Bernie Liu
// UWNetID: bliu29
// Homework 3
// 22/10/2024
// Combines the contents of a set of files



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int check_arguments(int argc, char *argv[], int *option);
void process_file(char *filename, int option, int *total);

// Checks if arguments are valid
// Parameters:
//  argc, *argv[]: cmd prompt arguments
//  *option: needed to account for argument length
// Returns:
//  int: start index of cmd prompt arguments to loop through
int check_arguments(int argc, char *argv[], int *option) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: ./wordcount requires an input file.\n");
        return -1;
    }

    int i=2;

    if (argv[1][0]=='-') {
        if (strncmp(argv[1], "-l", 500)==0) {
            *option=1;
        } else if (strncmp(argv[1], "-w", 500)==0) {
            *option=2;
        } else if (strncmp(argv[1], "-c", 500)==0) {
            *option=3;
        } else {
            i=1;
        }
        if (*option>0 && argc <= 2) {
            fprintf(stderr, "Usage: ./wordcount requires an input file.\n");
            return -1;
        }
    } else {
        i=1;
    }
    return i;
}

// Counts lines, words, and chars depending on the option
// Parameters:
//  *filename: filename
//  option: range of (0,3)
//  *total: total number of lines

void process_file(char *filename, int option, int *total) {
    FILE *file=fopen(filename, "r");

    if (!file) {
        fprintf(stderr, "%s will not open. Skipping\n", filename);
        return;
    } else {
        int lines=0;
        int words=0;
        int chars=0;
        char buffer[500]= {0};
        while (fgets(buffer, 500, file)) {
            lines++;
            (*total)++;
            chars+=strlen(buffer);
            int space=1;

            for (int j=0; j<strlen(buffer); j++) {
                if (isspace(buffer[j])) {
                    space=1;
                } else {
                    if (space == 1) {
                        words+=1;
                    }
                    space=0;
                }
            }
        }

        if (option==1) {
            printf("%d\n", lines);
        } else if (option == 2) {
            printf("%d\n", words);
        } else if (option == 3) {
            printf("%d\n", chars);
        } else {
            printf("%d %d %d %s\n", lines, words, chars, filename);
        }

        fclose(file);
    }
}

int main(int argc, char *argv[]) {
    int option=0;
    int total=0;
    int start=check_arguments(argc, argv, &option);
    if (start==-1) {
        return 1;
    }

    for (int i=start; i<argc; i++) {
        process_file(argv[i], option, &total);
    }
    if (option==0) {
        printf("Total Lines = %d\n", total);
    }
    return 0;
}



