// Copyright 2024 Bernie Liu
// Author: Bernie Liu
// UWNetID: bliu29
// Homework 6
// 15/11/2024
// Implements the methods of T9 predictive text

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "t9_lib.h"

// A specific trie node
typedef struct T9 {
    char* data;
    struct T9* children[10];
} T9;

T9* InitializeEmptyT9();
T9* InitializeFromFileT9(const char* filename);
void AddWordToT9(T9* dict, const char* word);
char* PredictT9(T9* dict, const char* nums);
void DestroyT9(T9* dict);
int number(char c);

// Creates an empty T9 dictionary and returns it
T9* InitializeEmptyT9() {
    T9* trie = (T9*)malloc(sizeof(T9));
    if (trie==NULL) {
        fprintf(stderr, "Not enough memory to make a new trie");
        return NULL;
    }

    trie->data=NULL;

    for (int i = 0; i < 10; i++) {
        trie->children[i] = NULL;
    }

    return trie;
}

// Creates a T9 dictionary from a file where each line is a word
// Parameters:
//  char* filename: the file to read the words are.
//  Each line should contain one word
// Returns:
//  T9*: pointer to a T9 dictionary
T9* InitializeFromFileT9(const char* filename) {
    FILE *file=fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "%s will not open\n", filename);
        return NULL;
    }
    char buffer[50]= {0};

    T9* trie=InitializeEmptyT9();
    if (trie==NULL) {
        fprintf(stderr, "Not enough memory to make a new trie");
        fclose(file);
        return NULL;
    }

    // For each word in the file, add it to the dictionary
    while (fgets(buffer, 50, file)) {
        if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer)-1] = '\0';
        }
        AddWordToT9(trie, buffer);
    }

    fclose(file);

    return trie;
}

// Adds a word to a T9 dictionary
// Parameters:
//  T9* dict: the dictionary to add the word to
//  const char* word: the word to add
// Returns if an error has occurred
void AddWordToT9(T9* dict, const char* word) {
    if (strlen(word)==0) {
        return;
    }
    T9* curr=dict;

    char* cpy = (char*)malloc(sizeof(char) * (strlen(word)+1));
    if (cpy==NULL) {
        fprintf(stderr, "Not enough memory to add %s", word);
        return;
    }
    strncpy(cpy, word, strlen(word)+1);

    // For each character in the word, build the trie
    for (int i=0; i<strlen(cpy); i++) {
        int n=number(cpy[i]);
        if (n==1) {
            fprintf(stderr, "%s is an invalid word\n", cpy);
            free(cpy);
            return;
        }
        if (curr->children[n] == NULL) {
            curr->children[n] = InitializeEmptyT9();
            if (curr->children[n] == NULL) {
                fprintf(stderr, "Not enough memory to make a new trie");
                free(cpy);
                return;
            }
        }
        curr=curr->children[n];
    }

    // children[0] is #
    // For when the word shares the same T9 representation as other words
    while (curr->data != NULL) {
        if (strcmp(cpy, curr->data)==0) {
            free(cpy);
            return;
        }
        if (curr->children[0] == NULL) {
            curr->children[0] = InitializeEmptyT9();
            if (curr->children[0] == NULL) {
                fprintf(stderr, "Not enough memory to make a new trie");
                free(cpy);
                return;
            }
        }
        curr=curr->children[0];
    }
    curr->data=cpy;
}

// Given the T9 represenation of a word, return the word from a T9 dictionary
char* PredictT9(T9* dict, const char* nums) {
    if (nums==NULL) {
        return NULL;
    }
    T9* curr=dict;
    for (int i=0; i<strlen(nums); i++) {
        int n;
        if (nums[i]=='#') {
            n=0;
        } else {
            n=nums[i] - '0';
        }

        if (!(n==0 || (n>=2 && n<=9))) {
            fprintf(stderr, "Error during prediction\n");
            return NULL;
        }
        if (curr->children[n] == NULL) {
            return NULL;
        }

        curr=curr->children[n];
    }
    return curr->data;
}

// Destroys a T9 dictionary, freeing up space using recursion
void DestroyT9(T9* dict) {
    for (int i=0; i<10; i++) {
        if (dict->children[i]!=NULL) {
            DestroyT9(dict->children[i]);
        }
    }
    if (dict->data != NULL) {
        free(dict->data);
    }
    free(dict);
}

// returns the T9 representation of a given letter
int number(char c) {
    if (c>=97 && c<=99) {
        return 2;
    }
    if (c>=100 && c<=102) {
        return 3;
    }
    if (c>=103 && c<=105) {
        return 4;
    }
    if (c>=106 && c<=108) {
        return 5;
    }
    if (c>=109 && c<=111) {
        return 6;
    }
    if (c>=112 && c<=115) {
        return 7;
    }
    if (c>=116 && c<=118) {
        return 8;
    }
    if (c>=119 && c<=122) {
        return 9;
    }
    // if the character is not a valid one
    return 1;
}

