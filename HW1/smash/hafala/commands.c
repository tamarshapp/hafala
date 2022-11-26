//		commands.c
//********************************************
#include "commands.h"
#include <stdlib.h>
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(list jobs, char* lineSize, char* cmdString)
{
	itrator get_job(list jobs, char* job_id);
	int max_job(list jobs);
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		if(num_arg > 1){
			fprintf(stderr, ‫‪"smash‬‬ ‫‪error:‬‬ ‫‪cd:‬‬ ‫‪too‬‬ ‫‪many‬‬ ‫‪arguments‬‬");
		}
		
		char* curr_dir = getcmd();
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		printf(getcmd());
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
 		if ((num_arg != 2) || ((int)arg[1] >= 0)){ 																										//check what is a illegal format
 			fprinf(strerr, "‫‪smash‬‬ ‫‪error:‬‬ ‫‪kill:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬");
 		}
 		std::list<char**>::iterator it = get_job(jobs, (char*)arg[2]);
 		if (it != NULL){
 			kill(arg[1], (int)((*it)[2]));																												//what is the place of pid
 		}
 		else {
 			printf("smash error: kill: job-id %d does not exist", arg[2]);
 		}
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		std::list<char**>::iterator itr = jobs.begin();
		for (it = jobs.begin(); it != jobs.end(); ++it){
			if (!strcmp((*it)[4] , "stopped")){
				print("[%d] %s : %d %d (stopped)", (int)(*it)[0], (*it)[1], (int)(*it)[2], difftime(time(), (int)(*it)[2]));
			}
			else{
				print("[%d] %s : %d %d", (int)(*it)[0], (*it)[1], (int)(*it)[2], difftime(time(), (int)(*it)[2]));	
			}
			//check if kob finish and delete
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		printf(‫‪"smash‬‬ ‫‪pid‬‬ ‫‪is‬‬ ‫‪%d", (int)getpid());
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					
					/* 
					your code
					*/
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
//int ExeComp(char* lineSize)
//{
//	char ExtCmd[MAX_LINE_SIZE+2];
//	char *args[MAX_ARG];
//    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
//    {
//		// Add your code here (execute a complicated command)
//					
//		/* 
//		your code
//		*/
//	} 
//	return -1;
//}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}

