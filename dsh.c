/*
 * dsh.c
 *
 *  Created on: Feb 23, 2024
 *      Author: Brayden Sawyer
 */
#include "dsh.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>


// TODO: Your function definitions below (declarations in dsh.h)

/**
 * isEmpty function to check if a string is empty
 */
int isEmpty(const char *s) {
    while(*s != '\0') {
        if(!isspace((unsigned char) *s)) {
            return 0;
        }
        s++;
    }
    return 1;
} 

/**
 * checkLast function to see if the last element is '&'
 */
int checkLast(char** e) {
    int i = 0;
    while(e[i] != NULL) {
        i++;
    }
    if(strcmp(e[i-1], "&") == 0) {
        return 1;
    }
    return 0;
}

//splits string into array based off a delim
char** split(char* s, char* delim){
    if(isEmpty(s)){
        return NULL;
    }
    //find length of array
    int len = 1; 
    for(int i = 0; i < strlen(s); i++){
        if(s[i] == *delim){
            len++;
        }
    }
    char **tokens = (char**)malloc((len + 1) * sizeof(char*));
    //malloc all char*s inside tokens
    for (int i = 0; i < len; i++) {
        tokens[i] = (char*) malloc(128 * sizeof(char));
    }

    // get the first token 
    char *token = strtok(s, delim);
   
    int i = 0; 
    // walk through other tokens 
    while( token != NULL) {
        strcpy(tokens[i], token);
        token = strtok(NULL, delim);
        i++;
    }
    //NULL terminate last element
    tokens[i] = NULL;
    return tokens; 
}
