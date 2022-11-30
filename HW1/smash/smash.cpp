/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <signal.h>
#include <list>
#include "commands.h"
#include "signals.h"
#include <time.h>
#define MAX_LINE_SIZE 80
#define MAXARGS 20
using namespace std;

char* L_Fg_Cmd;
std::list<job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 


//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];
    struct sigaction actcz = {0};
    actcz.sa_handler = &catch_sig;
    sigaction(SIGINT, &actcz, NULL);
	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals 


	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	int quit = 1;
	job fg_cur;
	fg_cur.job_id = 0;
	char* cd = NULL;
	
    	while (quit == 1)
    	{
	 	cout << "smash > ";
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// background command	
	 	if(!BgCmd(lineSize, jobs, cmdString, quit, fg_cur, cd)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString, quit, fg_cur, cd, 0);
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

