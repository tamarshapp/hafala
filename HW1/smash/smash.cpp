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
list<job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE];
job fg_cur;
string TAM ="first";

//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];
    struct sigaction actcz = {0};
    actcz.sa_handler = &catch_sig;
    actcz.sa_flags = SA_RESTART;
    sigaction(SIGINT, &actcz, NULL);
    sigaction(SIGTSTP, &actcz, NULL);
    fg_cur.job_id = 0;
	fg_cur.command = "-";
	fg_cur.pid = 0;
	fg_cur.seconds_elapsed = 0;
	fg_cur.stopped = 0;

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
	if (L_Fg_Cmd == NULL){
		exit (-1);
	}
	L_Fg_Cmd[0] = '\0';
	int quit = 1;
    while (quit == 1)
    {
    	cout << "smash > "<<endl;
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// background command	
	 	if(!BgCmd(lineSize, cmdString, quit)) continue; 
					// built in commands
		ExeCmd(lineSize, cmdString, quit, 0);
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

