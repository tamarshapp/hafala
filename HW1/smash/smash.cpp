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
#include <time.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20
using namespace std;

char* L_Fg_Cmd;
list<job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE];
job fg_cur;
int hold_job=0;
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
    int sig_int = sigaction(SIGINT, &actcz, NULL);
    if (sig_int == -1){
    	perror("smash error: sigaction failed");
    	exit(-1);
    }
    int sig_stp = sigaction(SIGTSTP, &actcz, NULL);
    if (sig_stp == -1){
    	perror("smash error: sigaction failed");
    	exit(-1);
    }
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


	
	L_Fg_Cmd =new char [MAX_LINE_SIZE+1];
	if (L_Fg_Cmd == NULL){
    	perror("smash error: malloc failed");
		exit (-1);
	}
	L_Fg_Cmd[0] = '\0';
	int quit = 1;
    while (quit == 1)
    {
    	cout << "smash > "<<flush;
		char* get = fgets(lineSize, MAX_LINE_SIZE, stdin);
		if (get == NULL){
	    	perror("smash error: fgets failed");
	    	exit(-1);
		}
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// background command
		check_list();
	 	if(!BgCmd(lineSize, cmdString, quit)) continue; 
					// built in commands
		ExeCmd(lineSize, cmdString, quit, 0);
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

