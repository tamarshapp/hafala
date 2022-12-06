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
			kill(fg_cur.pid, SIGINT);
			fg_cur.job_id = 0;
			std::cout << "smash: process " <<old<< " was killed" <<endl;
		}
	}
	else if (sig == SIGTSTP){
	    cout << "smash: caught ctrl-Z"<<endl;
	    if(fg_cur.job_id != 0) {
	        pid_t old = fg_cur.pid;
	        kill(fg_cur.pid, SIGSTOP);
	        time(&(fg_cur.seconds_elapsed));
	        fg_cur.stopped = 1;
	        fg_cur.job_id = max_job(jobs)+1;
	        jobs.push_back(fg_cur);
	        fg_cur.job_id = 0;
	        std::cout << "smash: process " <<old<< " stopped" << endl;
	    }
	}
	return;
}
