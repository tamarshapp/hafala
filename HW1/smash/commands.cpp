//		commands.c
//********************************************
#include "commands.h"
//#include "signals.h"
#include <signal.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <list>
#include <iterator>
#include <string>
using namespace std;
//********************************************



// function name: get_job
// Description: bing the job matching id
// Parameters: pointer to jobs, job id string
// Returns: iterator in the job place
list<job>::iterator get_job(int job_id, int& no_job){
	list<job>::iterator it;
	for (it = jobs.begin(); it != jobs.end(); it++){
		if(it->job_id == job_id){
			return it;
		}
	}
	no_job = 1;
	return it;
}

// function name: max_job
// Description: go over list and bring back max_gob num
// Parameters: pointer to jobs
// Returns: int max val
int max_job(list<job> jobs){
	list<job>::iterator it;
	int max = 0;
	for (it = jobs.begin(); it != jobs.end(); ++it){
		if(it->job_id > max){
			max = it->job_id;
		}
	}
	return max;
}

//bool check_built_in(char* command){
//	if ((!strcmp(cmd, "cd")) || (!strcmp(cmd, "pwd")) || (!strcmp(cmd, "kill")) || (!strcmp(cmd, "jobs")) || 
//			(!strcmp(cmd, "diff")) || (!strcmp(cmd, "fg")) || (!strcmp(cmd, "bg")) || (!strcmp(cmd, "showpid")) || (!strcmp(cmd, "quit"))){
//		return true;
//	}
//	return false;
//}

bool check_if_digit(char* in){
	for(int i = 0; i != sizeof(in) - 1; i++){
		if ((int)in[i] == 0){
			break;
		}
		if (((int)in[i] < 48) || ((int)in[i] > 57)){
			return false;
		}
	}
	return true;
}

void check_list(){
	list<job>::iterator it = jobs.begin();
	int status1 = 1;
	while (it != jobs.end()){
		pid_t w = waitpid(it->pid, &status1, WNOHANG);
		//proccess die
		if (w != 0){
			it = jobs.erase(it);
		}
		else{
			it++;
		}
	}
//		else if (WIFSIGNALED(status1)){
//			it = jobs.erase(it);
////		}
//		else{
//			it++;
//		}
//	}
	return;
}

// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString, int &quit, bool bg)
{
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	const char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    cmd = strtok(lineSize, delimiters);
	if (cmd == NULL){
		return 0; 
	}
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
	if (!strcmp(cmd,"cd") ) 
	{
		string TAM_curr = getcwd(pwd,MAX_LINE_SIZE);
		const char* curr_cd = TAM_curr.c_str();
		if (curr_cd == NULL){
			perror("smash error: getcwd failed");
			return 1;
		}
		const char *former = TAM.c_str();
		if (num_arg > 1){
			cout << "smash error: cd: too many arguments"<<endl;
			return 1;
		}
		else if (!strcmp(args[1], "-")){
			if (!strcmp(former,"first")){
				cout << "smash error: cd: OLDPWD not set"<<endl;
				return 1;
			}
			else {
				int c = chdir(former);
				if (c != 0){
					perror("smash error: chdir failed");
					return 1;
				}
			}
		}
		else{
			int c = chdir(args[1]);
			if (c != 0){
				perror("smash error: chdir failed");
				return 1;
			}		
		}
		TAM = (string)TAM_curr;				
	} 
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		char* pwd_curr = getcwd(pwd,MAX_LINE_SIZE);
		//int res_pwd = pwd_curr.compare(NULL);
		if (pwd_curr == NULL){
			perror("smash error: getcwd failed");
			return 1;
		}
		cout<<pwd_curr<<endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		int no_job = 0;
		if ((num_arg == 2) && (check_if_digit(args[2]) == false)){
			cout<<"‫‪smash‬‬ ‫‪error:‬‬ ‫‪kill:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬"<<endl;
		
		}
		else if ((num_arg != 2) || (atoi(args[1]) >= 0)){ 																										//check what is a illegal format and where to print
 			cout<<"‫‪smash‬‬ ‫‪error:‬‬ ‫‪kill:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬"<<endl;
 		}
		else {
			std::list<job>::iterator it;
			it = get_job(atoi(args[2]), no_job);
			if (no_job == 0){
				//cheack if need without the -
				int kill_send = kill(it->pid, -(atoi(args[1])));
				if (kill_send == -1){
					perror("smash error: kill failed");
					return 1;
				}
				cout<<"signal number " << -(atoi(args[1]))<< " was sent to pid "<<it->pid<<endl;
			}
			else {
				cout<<"smash error: kill: job-id " <<args[2]<< " does not exist"<<endl;
			}
		}	
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		time_t time_now;
		check_list();
		std::list<job>::iterator it = jobs.begin();
		while(it != jobs.end()){
			time(&time_now);
			if (time_now == (time_t)(-1)){
				perror("smash error: time failed");
				return 1;
			}
			if (it->stopped){
				cout<<"["<<it->job_id<<"] "<<it->command<<" : "<<it->pid<<" "<< difftime(time_now, it->seconds_elapsed)<<" secs"<<" (stopped)"<<endl;
			}
			else{
				cout<<"["<<it->job_id<<"] "<<it->command<<" : "<<it->pid<<" "<<difftime(time_now, it->seconds_elapsed)<<" secs"<<endl;
			}
			it++;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		cout<<"smash pid is "<<(int)getpid()<<endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		int status_fg;
		bool id_exists = false;
		if ((num_arg == 1) && !(check_if_digit(args[1]))){
			cout<<"smash error: fg: invalid arguments"<<endl;
			return 1;
		}
		else if ((num_arg == 1) && (jobs.empty())){
			cout<<"smash error: fg: job-id "<<atoi(args[1])<<" does not exist"<<endl;
			return 1;
		}
		else if (num_arg == 1){
			list<job>::iterator it;
			for(it=jobs.begin();it!=jobs.end();it++)
			{
				if(it->job_id==atoi(args[1]))//might need to make sure args[1] exists
				{
					id_exists=true;
					cout<<it->command<<" : "<<it->pid<<endl;
					fg_cur=*it;
					int curr_fg = kill(it->pid,SIGCONT);
					if (curr_fg == -1){
						perror("smash error: kill failed");
						return 1;
					}
					jobs.erase(it);
					hold_job = fg_cur.job_id;
					int w_fg = waitpid(it->pid,&status_fg,WUNTRACED);
					hold_job=0;
					if (w_fg == -1){
						perror("smash error: waitpid failed");
						return 1;
					}
					fg_cur.job_id = 0;
					break;
				}
			}
			if (!id_exists){
				cout<<"smash error: fg: job-id "<<args[1]<<" does not exist"<<endl;
			}
		}
		else if (num_arg == 0){
			if (jobs.empty()){
				cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪fg:‬‬ ‫‪jobs‬‬ ‫‪list‬‬ ‫‪is‬‬ ‫‪empty‬‬"<<endl;
			}
			else{
				int max_id = max_job(jobs);
				list<job>::iterator it;
				for(it=jobs.begin();it!=jobs.end();it++)
				{
					if(it->job_id==max_id)//might need to make sure args[1] exists
					{
					cout<<it->command<<" : "<<it->pid<<endl;
					fg_cur=*it;
					int curr_fg = kill(it->pid,SIGCONT);
					if (curr_fg == -1){
						perror("smash error: kill failed");
						return 1;
					}
					jobs.erase(it);
					hold_job=fg_cur.job_id;
					int w_fg = waitpid(it->pid,&status_fg,WUNTRACED);
					hold_job=0;
					if (w_fg == -1){
						perror("smash error: waitpid failed");
						return 1;
					}
					fg_cur.job_id = 0;
					break;
					}
				}
			}
		}
		else{
			cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪fg:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬"<<endl;
			return 1;
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		int no_job = 0;
		//if we get the job id
		if ((num_arg == 1) && !(check_if_digit(args[1]))){
			cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪bg:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬"<<endl;
			return 1;
		}
		else if (num_arg == 1){
			list<job>::iterator it = get_job(atoi(args[1]), no_job);
			if (no_job == 1){
				cout<<"smash error: bg: job-id "<< atoi(args[1]) << " does not exist"<<endl;
			}
			else if(it->stopped == 0){
				cout<<"smash error: bg: job-id "<< args[1]<< " is already running in the background"<<endl;
			}
			else {
				int kill_bg = kill(it->pid, SIGCONT);
				if (kill_bg == -1){
					perror("smash error: kill failed");
					return 1;
				}
				list<job>::iterator it2;
				for(it2=jobs.begin(); it2!=jobs.end(); it2++){
					if(it2->job_id == atoi(args[1])){
						cout<< it2->command<<" : "<<it2->pid<<endl;
						it2->stopped = false;
					}
				}
				//it->stopped = false;
			}
		}
		//if we didnt get the job id
		else if (num_arg == 0){
			//get the job with the max id and stopped
			int max_job_id_stopped = 0;
			std::list<job>::iterator it;
			for (it = jobs.begin(); it != jobs.end(); ++it){
				if((it->job_id > max_job_id_stopped) && (it->stopped)){
					max_job_id_stopped = it->job_id;
				}
			}
			//if there isnt job stopped at the list
			if (max_job_id_stopped == 0){
				cout<<"‫‪smash‬‬ ‫‪error:‬‬ ‫‪bg:‬‬ ‫‪there‬‬ ‫‪are‬‬ ‫‪no‬‬ ‫‪stopped‬‬ ‫‪jobs‬‬ ‫‪to‬‬ ‫‪resume‬‬"<<endl;
			}
			else{
				std::list<job>::iterator it = get_job(max_job_id_stopped, no_job);
				int kill_bg = kill(it->pid, SIGCONT);
				if (kill_bg == -1){
					perror("smash error: kill failed");
					return 1;
				}
				list<job>::iterator it2;
				for(it2=jobs.begin(); it2!=jobs.end(); it2++){
					if(it2->job_id == max_job_id_stopped){
						cout<<it2->command<<" : "<<it2->pid<<endl;
						it2->stopped = false;
					}
				}
			}
		}
		else{
			cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪bg:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬"<<endl;
			return 1;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if ((num_arg == 1) && (!strcmp(args[1], "kill"))){
			std::list<job>::iterator it;
			int status_quit;
			check_list();
			for (it = jobs.begin(); it != jobs.end(); ++it){
				int kill_quit = kill(it->pid, SIGTERM);
				cout<<"["<<it->job_id<<"] "<<it->command  <<" - Sending SIGTERM..."<<flush;
				if (kill_quit == -1){
					perror("smash error: kill failed");
					return 1;
				}
				sleep(5);
				int w_quit = waitpid(it->pid, &status_quit, WNOHANG);
				if (w_quit == -1){
					perror("smash error: waitpid failed");
					return 1;
				}
				if (WIFEXITED(status_quit) || WIFSIGNALED(status_quit)){
					cout<<" Done."<<endl;
				}
				else{
					int kill_sig = kill(it->pid, SIGKILL);
					if (kill_sig == -1){
						perror("smash error: kill failed");
						return 1;
					}
					cout<<" (5 sec passed) Sending SIGKILL... Done."<<endl;
				}
			}	
			quit = 0;
		}
		else {
			quit = 0;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "diff")){
		bool not_equ = 0;
		if (num_arg !=2){
			cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫‪diff:‬‬ ‫‪invalid‬‬ ‫‪arguments‬‬"<<endl;
		}
		else{
			FILE* f1 = fopen(args[1], "r");
			if (f1 == NULL){
				perror("smash error: fopen failed");
				return 1;
			}
			FILE* f2 = fopen(args[2], "r");
			if (f2 == NULL){
				perror("smash error: fopen failed");
				return 1;
			}
			int N = 10000;
			char* buf1;
			buf1 = new char [N];
			char* buf2;
			buf2 = new char [N];
			do {
				size_t r1 = fread(buf1, 1, N, f1);
			    size_t r2 = fread(buf2, 1, N, f2);
			    if (ferror(f1) || ferror(f1)){
			    	perror("smash error: fread failed");
			    	delete [] buf1;
			    	delete [] buf2;
			    	return 1;
			    }
			    if (r1 != r2 ||memcmp(buf1, buf2, r1)) {
			    	not_equ=1;
			    	cout << 1 <<endl;
					break;
			    	  // Files are not equal
					}
			} while (!feof(f1) && !feof(f2));
			if(!not_equ){
				cout<<!(feof(f1) && feof(f2))<<endl;		
			}
			int f1_close = fclose(f1);
			int f2_close = fclose(f2);
			if ((f1_close != 0) || (f2_close != 0)){
				perror("smash error: fclose failed");
				delete [] buf1;
				delete [] buf2;
				return 1;
			}
			delete [] buf1;
			delete [] buf2;
		}
	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString, bg);
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
void ExeExternal(char *args[MAX_ARG], char* cmdString , bool bg)
{
	int status;
	int pID;
    switch(pID = fork()) 
    {
    		case -1: 
					// Add your code here (error)
                     perror("smash error: fork failed");
                     exit(1);
        	case 0 :
        			
                	// Child Process
               		setpgrp();              	
                    execv(args[0] , args);
			        perror("smash error: exec failed");
                    exit(1);
			default:
                	// Add your code here
                    if(bg == 0){
        				//waitpid(pID, &status, WNOHANG | WUNTRACED);
                    	fg_cur.job_id = -1;
                    	fg_cur.command = cmdString;
                    	fg_cur.pid = pID;
                    	time(&(fg_cur.seconds_elapsed));
                    	if(fg_cur.seconds_elapsed == (time_t)(-1)){
                    		perror("smash error: time failed");
                    		return;
                    	}
						fg_cur.stopped = 0;
                    	int w_ext = waitpid(pID, &status, WUNTRACED);
                    	if (w_ext == -1){
        					perror("smash error: waitpid failed");
        					return;
                    	}
                    	fg_cur.job_id = 0;
                    }
                    else{
                        job bg_job;
                        check_list();
                        bg_job.job_id = max_job(jobs)+1;
                        bg_job.command = cmdString;
                        bg_job.pid = pID;
                        time(&(bg_job.seconds_elapsed));
                        if (bg_job.seconds_elapsed == (time_t)(-1)){
                    		perror("smash error: time failed");
                    		return;
                        }
                        bg_job.stopped = 0;
                        check_list();
                        jobs.push_back(bg_job);
                    }
	}
}

//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize,  char* cmdString, int &quit)
{
    bool bg = 0;
	char* Command;
	const char* delimiters = " \t\n";
	char* original_line;
	original_line = new char [strlen(lineSize)-2];
	if (lineSize[strlen(lineSize)-2] == '&'){
        bg = 1;
		lineSize[strlen(lineSize)-2] = '\0';
		strcpy(original_line,lineSize); 
    	Command = strtok(lineSize, delimiters);
        if (Command == NULL)  {
        	delete [] original_line; 
		    return -1;
        }
        ExeCmd(original_line, cmdString, quit, bg); //try
        delete [] original_line;
        return 0;
	}
	delete [] original_line;
	return -1;
}


