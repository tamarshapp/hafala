// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include "commands.h"
#include <iostream>
#include <list>
using namespace std;


void catch_sig(int sig) {
	if (fg_cur.job_id==0){
		return;
	}
	if (sig == SIGINT){
		cout << "smash: caught ctrl-C"<<endl;
		if(fg_cur.job_id != 0){
			pid_t old = fg_cur.pid;
			int kill_sigit = kill(fg_cur.pid, SIGINT);
			if(kill_sigit == -1){
				perror("smash error: kill failed");
				return;
			}
			fg_cur.job_id = 0;
			std::cout << "smash: process " <<old<< " was killed" <<endl;
		}
	}
	else if (sig == SIGTSTP){
	    cout << "smash: caught ctrl-Z"<<endl;
	    if(fg_cur.job_id != 0) {
	        pid_t old = fg_cur.pid;
	        int kill_sigtstp = kill(fg_cur.pid, SIGSTOP);
	        if(kill_sigtstp == -1){
				perror("smash error: kill failed");
				return;
	        }
	        time(&(fg_cur.seconds_elapsed));
	        if(fg_cur.seconds_elapsed){
				perror("smash error: time failed");
				return;
	        }
	        fg_cur.stopped = 1;
	        if(hold_job!=0){
	        	fg_cur.job_id = hold_job;
	        }
	        else{
	        	 fg_cur.job_id =max_job(jobs)+1; 	
	        }
	        hold_job = 0;
	        jobs.push_back(fg_cur);
	        fg_cur.job_id = 0;
	        std::cout << "smash: process " <<old<< " stopped" << endl;
	    }
	}
	return;
}
