//		commands.c
//********************************************
#include "commands.h"
#include "signals.h"
#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <list>
using namespace std;
//********************************************



// function name: get_job
// Description: bing the job matching id
// Parameters: pointer to jobs, job id string
// Returns: iterator in the job place
std::list<job>::iterator get_job(list<job> jobs, int job_id){

	std::list<job>::iterator it;
	for (it = jobs.begin(); it != jobs.end(); ++it){
		if(it->job_id == job_id){
			return it;
		}
	}
	return jobs.end();

}

// function name: max_job
// Description: go over list and bring back max_gob num
// Parameters: pointer to jobs
// Returns: int max val
int max_job(list<job> jobs){
	std::list<job>::iterator it;
	int max = 0;
	for (it = jobs.begin(); it != jobs.end(); ++it){
		if(it->job_id > max){
			max = it->job_id;
		}
	}
	return max;
}

// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(list<job> jobs, char* lineSize, char* cmdString, int quit, job fg_cur, char* cd,  bool bg)
{
	char* cmd;
	char* cmd_fg[MAX_ARG];
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
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
		char* cd_curr = getcwd(pwd,MAX_LINE_SIZE);
		if (num_arg > 2){
			cout << ‫‪"smash‬‬ ‫‪error:‬‬ ‫‪cd:‬‬ ‫‪too‬‬ ‫‪many‬‬ ‫‪arguments‬‬"<<endl;
		}
		else if (!strcmp((char*)args[1]), "-"){
			if (cd == NULL){
				cout << "smash error: cd: OLDPWD not set"<<endl;
			}
			else{
				chdir(cd);
				cd = cd_curr;
			}
		}
		else if((!strcmp((char*)args[1]), "..")){
			chdir("..");
			cd = cd_curr;
		}
		else{
			chdir((char*)args[1]);
			cd = cd_curr;
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		printf(getcwd(pwd,MAX_LINE_SIZE));
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
 		if ((num_arg != 2) || (atoi(args[1]) >= 0)){ 																										//check what is a illegal format and where to print
 			fprintf(stdout, "‫‪smash‬‬ ‫‪error:‬‬ ‫‪kill:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬");
 		}
 		std::list<job>::iterator it = get_job(jobs, atoi(args[2]));
 		if (it != jobs.end()){
 			kill(it->pid, atoi(args[1]));																												//what is the place of pid
 		}
 		else {
 			fprintf(stdout, "smash error: kill: job-id %d does not exist", args[2]);
 		}
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		time_t* time_now = NULL;
		std::list<job>::iterator it = jobs.begin();
		while(it != jobs.end()){
			if (kill(it->pid, 0) == 0){
				if (it->stopped){
					fprintf(stdout, "[%d] %s : %d %d (stopped)", it->job_id, it->command, it->pid, difftime(time(time_now), it->seconds_elapsed));
				}
				else{
					fprintf(stdout, "[%d] %s : %d %d", it->job_id, it->command, it->pid, difftime(time(time_now), it->seconds_elapsed));
				}
				it++;
			}
			else{
				it = jobs.erase(it);
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		fprintf(‫stdout, ‪"smash‬‬ ‫‪pid‬‬ ‫‪is‬‬ ‫‪%d", (int)getpid());
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		int status;
		if (num_arg == 2){
			std::list<job>::iterator it = get_job(jobs, atoi(args[2]));
			if (it == jobs.end()){
				fprintf(stdout, "smash error: fg: job-id %d does not exist", atoi(args[2]));
			}
			else {
				kill(it->pid, ‫‪SIGCONT‬‬);
				fg_cur = (const job&)it;
				waitpid(it->pid, &status, WNOHANG | WUNTRACED);
				/*if (WIFSTOPPED(status)){
					catch_kill(fg_cur);
				}
				else if(WIFSIGNALED(status)){
					catch_sleep(jobs, fg_cur);
				}*/
				fg_cur.job_id = 0;
			}
		}
		else if (num_arg == 1){
			int max_job_id = max_job(jobs);
			if (max_job_id == 0){
				cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪fg:‬‬ ‫‪jobs‬‬ ‫‪list‬‬ ‫‪is‬‬ ‫‪empty‬‬";
			}
			else{
				std::list<job>::iterator it = get_job(jobs, max_job_id);
				kill(it->pid, ‫‪SIGCONT‬‬);				
				fg_cur = (const job&)it;
				waitpid(it->pid, &status, WNOHANG | WUNTRACED);
				/*if (WIFSTOPPED(status)){
					catch_kill(fg_cur);
				}
				else if(WIFSIGNALED(status)){
					catch_sleep(jobs, fg_cur);
				}*/
				fg_cur.job_id = 0;
			}
		}
		else{
			cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪fg:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬";
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		//if we get the job id
		if (num_arg == 2){
			std::list<job>::iterator it = get_job(jobs, atoi(args[2]));
			if (it == jobs.end()){
				fprintf(stdout, "smash error: bg: job-id %d does not exist", atoi(args[2]));
			}
			else if(it->stopped == 1){
				fprintf(stdout, "‫‪smash‬‬ ‫‪error:‬‬ ‫‪bg:‬‬ ‫‪job-id‬‬ ‫‪%d ‫‪is‬‬ ‫‪already‬‬ ‫‪running‬‬ ‫‪in‬‬ ‫‪the‬‬ ‫‪background‬‬", atoi(args[2]));
			}
			else {
				kill(it->pid, ‫‪SIGCONT‬‬);																										//what is the place of pid
				it->stopped = 0;
			}
		}
		//if we didnt get the job id
		else if (num_arg == 1){
			//get the job with the max id and stopped
			int max_job_id_stopped = 0;
			std::list<job>::iterator it;
			for (it = jobs.begin(); it != jobs.end(); ++it){
				if((it->job_id > max_job_id_stopped) && (it->stopped == 1)){
					max_job_id_stopped = it->job_id;
				}
			}
			//if there isnt job stopped at the list
			if (max_job_id_stopped == 0){
				cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪bg:‬‬ ‫‪there‬‬ ‫‪are‬‬ ‫‪no‬‬ ‫‪stopped‬‬ ‫‪jobs‬‬ ‫‪to‬‬ ‫‪resume‬‬";
			}
			else{
				std::list<job>::iterator it = get_job(jobs, max_job_id_stopped);
				kill(it->pid, ‫‪SIGCONT‬‬);																										//what is the place of pid
				it->pid = 0;
			}
		}
		else{
			cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪bg:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬";
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if (num_arg == 2 & (!strcmp((char*)args[1], "kill"))){
			std::list<job>::iterator it;
			for (it = jobs.begin(); it != jobs.end(); ++it){
				kill(it->pid, SIGTERM);
				sleep(5000);
				if (!kill(it->pid, 0)){
					fprintf(stdout, "[%d] %s - Sending SIGTERM... Done.", it->job_id, it->command);
				}
				else{
					kill(it->pid, SIGKILL);
					fprintf(stdout, "[%d] %s - Sending SIGTERM... (5 sec passed) Sending SIGKILL... Done.", it->job_id, it->command);
				}
			}
			quit = 0;
		}
		else {
			quit = 0;
		}
	}
	else if (!strcmp(cmd, "diff")){
		if (num_arg != 3){
			cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪diff:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬";
		}
		else{
			FILE* f1 = fopen(args[1], "r");
			FILE* f2 = fopen(args[2], "r");
			int N = 10000;
			char buf1[N];
			char buf2[N];

			do {
				size_t r1 = fread(buf1, 1, N, f1);
			    size_t r2 = fread(buf2, 1, N, f2);

			    if (r1 != r2 ||memcmp(buf1, buf2, r1)) {
			    	return 1;  // Files are not equal
			    	}
			} while (!feof(f1) && !feof(f2));
			return !(feof(f1) && feof(f2));
		}
	}
	/*else if (!strcmp(cmd, "^C")){
		catch_kill(fg_cur);
	}
	else if (!strcmp(cmd, "^Z")){
		catch_sleep(jobs, fg_cur);
	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString, bg, jobs, fg_cur);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		fprintf(stdout, "smash error: \"%s\"\n", cmdString);
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
void ExeExternal(char *args[MAX_ARG], char* cmdString , bool bg, list<job> jobs, job fg_cur)
{
	int status;
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
                     perror("error fork");
                     exit(1);
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
                    execv(args[0] , args );
			        perror("error exec");
                    exit(1);
			default:
                	// Add your code here
                    if(bg == 0){
        				waitpid(pID, &status, WNOHANG | WUNTRACED);
        				/*if (WIFSTOPPED(status)){
        					catch_kill(fg_cur);
        				}
        				else if(WIFSIGNALED(status)){
        					catch_sleep(jobs, fg_cur);
        				}*/
                    }
	}
}

//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, list<job> jobs,  char* cmdString, int quit, job fg_cur, char* cd)
{
    bool bg = 0;
	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	int status;
	if (lineSize[strlen(lineSize)-2] == '&'){
        bg = 1;
		lineSize[strlen(lineSize)-2] = '\0';
    	Command = strtok(lineSize, delimiters);
        if (Command == NULL)   {
		    return -1;
            }
		// Add your code here (execute a in the background)
        job bg_job;
        bg_job.job_id = max_job(jobs)+1;
        bg_job.command = Command;
        bg_job.pid = getpid();
        time(&(bg_job.seconds_elapsed));
        bg_job.stopped = 0;
        jobs.push_back(bg_job);
        ExeCmd(jobs, lineSize, Command, quit, fg_cur, cd ,bg);
        return 0;
	}
	return -1;
}


