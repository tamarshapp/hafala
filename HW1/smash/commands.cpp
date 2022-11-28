//		commands.c
//********************************************
#include "commands.h"
#include <stdlib.h>
//********************************************



// function name: get_job
// Description: bing the job matching id
// Parameters: pointer to jobs, job id string
// Returns: iterator in the job place
std::list<job>::iterator get_job(list jobs, int job_id){

	std::list<job>::iterator it;
	for (it = jobs.begin(); it != jobs.end(); ++it){
		if(it->id == job_id){
			return it;
		}
	}
	return NULL

}

// function name: max_job
// Description: go over list and bring back max_gob num
// Parameters: pointer to jobs
// Returns: int max val
int max_job(list jobs){
	std::list<job>::iterator it;
	int max = 0;
	for (it = jobs.begin(); it != jobs.end(); ++it){
		if(it->id > max){
			max = it->id;
		}
	}
	return max
}

// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(list jobs, char* lineSize, char* cmdString)
{
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
 		std::list<job>::iterator it = get_job(jobs, (int)arg[2]);
 		if (it != NULL){
 			kill(it->pid, (int)arg[1]));																												//what is the place of pid
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
				printf("[%d] %s : %d %d (stopped)", (int)(*it)[0], (*it)[1], (int)(*it)[2], difftime(time(), (int)(*it)[2]));
			}
			else{
				printf("[%d] %s : %d %d", (int)(*it)[0], (*it)[1], (int)(*it)[2], difftime(time(), (int)(*it)[2]));	
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
		if (num_arg == 2){
			std::list<job>::iterator it = get_job(jobs, (int)arg[2]);
			if (it == NULL){
				printf("smash error: fg: job-id %d does not exist", (int)arg[2]);
			}
			else {
				kill(it->pid, ‫‪SIGCONT‬‬);																										//what is the place of pid
				waitpid(it->pid);
			}
		}
		else if (num_arg == 1){
			max_job_id = max_job(list jobs);
			if (max_job_id == 0){
				printf("‫‪smash‬‬ ‫‪error:‬‬ ‫‪fg:‬‬ ‫‪jobs‬‬ ‫‪list‬‬ ‫‪is‬‬ ‫‪empty‬‬");
			}
			else{
				std::list<job>::iterator it = get_job(jobs, max_job_id);
				kill(it->pid, ‫‪SIGCONT‬‬);																										//what is the place of pid
				waitpid(it->pid);
			}
		}
		else{
			printf("‫‪smash‬‬ ‫‪error:‬‬ ‫‪fg:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬");
		}
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		//if we get the job id
		if (num_arg == 2){
			std::list<job>::iterator it = get_job(jobs, (int)arg[2]);
			if (it == NULL){
				printf("smash error: bg: job-id %d does not exist", (int)arg[2]);
			}
			else if(it->stopped == 1){
				printf("‫‪smash‬‬ ‫‪error:‬‬ ‫‪bg:‬‬ ‫‪job-id‬‬ ‫>‪<job-id‬‬ ‫‪is‬‬ ‫‪already‬‬ ‫‪running‬‬ ‫‪in‬‬ ‫‪the‬‬ ‫‪background‬‬");
			}
			else {
				kill(it->pid, ‫‪SIGCONT‬‬);																										//what is the place of pid
				it->stopprd = 0;
			}
		}
		//if we didnt get the job id
		else if (num_arg == 1){
			//get the job with the max id and stopped
			max_job_id_stopped = 0;
			std::list<job>::iterator it;
			for (it = jobs.begin(); it != jobs.end(); ++it){
				if((it->id > max) && (it->stoppes == 1)){
					max_job_id_stopped = it->id;
				}
			}
			//if there isnt job stopped at the list
			if (max_job_id == 0){
				printf("‫‪smash‬‬ ‫‪error:‬‬ ‫‪bg:‬‬ ‫‪there‬‬ ‫‪are‬‬ ‫‪no‬‬ ‫‪stopped‬‬ ‫‪jobs‬‬ ‫‪to‬‬ ‫‪resume‬‬");
			}
			else{
				std::list<job>::iterator it = get_job(jobs, max_job_id);
				kill(it->pid, ‫‪SIGCONT‬‬);																										//what is the place of pid
				it->pid = 0;
			}
		}
		else{
			printf("‫‪smash‬‬ ‫‪error:‬‬ ‫‪bg:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬");
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	}
	else if (!strycmp(cmd, "diff")){
		
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

