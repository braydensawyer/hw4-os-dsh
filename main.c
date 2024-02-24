/*
 * main.c
 *
 *  Created on: Feb 23 2024
 *      Author: Brayden Sawyer
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include "dsh.h"

#define MAX_PROC 250

int main(int argc, char **argv) {

	// DO NOT REMOVE THE BLOCK BELOW (FORK BOMB PREVENTION) //
	struct rlimit limit;
	limit.rlim_cur = MAX_PROC;
	limit.rlim_max = MAX_PROC;
	setrlimit(RLIMIT_NPROC, &limit);
	// DO NOT REMOVE THE BLOCK ABOVE THIS LINE //


	char* cmd = (char*)malloc(MAXBUF); // stores user input from commmand line
	int exit = 0;

	while(exit == 0) {
		//prompts user for input
		printf("dsh> ");
		fgets(cmd, MAXBUF, stdin);
			//deletes '\n'
			for(int i = 0; i < strlen(cmd); i++) {
				if(cmd[i] == '\n') {
					cmd[i] = '\0';
				}
			}
		
		
		//creates array from user input
		char** elements = split(cmd, " ");
		//Check if input is empty
		if(elements == NULL) {
			printf("String can not be empty\n");
		}
		//exit
		else if(strcmp(cmd, "exit") == 0) {
			exit = 1;
		}
		//get current directory
		else if(strcmp(elements[0], "pwd") == 0) {
			char* curDir = malloc(MAXBUF);
			getcwd(curDir, MAXBUF);
			printf("%s\n", curDir);
			free(curDir);
		}
		//change directory
		else if(strcmp(elements[0], "cd") == 0){
			//check if user gives you directory or not
			if(elements[1] == NULL){
				chdir(getenv("HOME"));
			}
			else{
				chdir(elements[1]);
			}
		}
		//mode 1
		else if(elements[0][0] == '/'){
			//check if file is valid
			if (access(elements[0], F_OK | X_OK) == 0) {
				//parent process
				if(0 != fork()){
					//only waits if last element is not &
					if(checkLast(elements) == 0){
						wait(NULL);
					}
				}
				//child process
				else{
					execv(elements[0], elements);
				}
			}
			else {
				printf("ERROR: %s not found!\n", elements[0]);
			}

		}
		//mode 2
		else {
			char* curDir = malloc(MAXBUF);
			//gets path 
			char** paths = split(getenv("PATH"), ":");
			int found = 0;
			int i = 0;
			//append user input to current directory
			getcwd(curDir, MAXBUF);
			strcat(curDir, "/");
			strcat(curDir, elements[0]);
			//loop paths from variable and make sure they are valid
			while(found == 0 && paths[i] != NULL) {
				//is file valid
				if(access(curDir, F_OK | X_OK) == 0) {
					found = 1;
					//parent
					if(0 != fork()) {
						//waits if last element is not '&'
						if(checkLast(elements) == 0) {
							wait(NULL);
						}
					}
					//child
					else {
						elements[0] = curDir;
						execv(curDir, elements);
					}
				}
				//append input to path
				strcpy(curDir, paths[i]);
			 	strcat(curDir, "/");
			 	strcat(curDir, elements[0]);
				i++;
			}
			if(found == 0) {
				printf("ERROR: %s not found!\n", elements[0]);
			}
			free(curDir);
			int j = 0;
			while(paths[j] != NULL) {
				free(paths[j]);
				paths[j] = NULL;
				j++;
			}
			free(paths);
		}
		int i = 0;
		while(elements[i] != NULL) {
			free(elements[i]);
			elements[i] = NULL;
			i++;
		}
		free(elements);
	}
	free(cmd);
	return 0;
}
