// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include "command.h"
#include <iostream>
#include <list>
using namespace std;

void catch_sig(int sig) {
	if (sig == SIGKILL){
		cout << "smash: caught ctrl-C"<<endl;
		if(fg.job_id != 0){
		    pid_t old = fg.pid;
		    kill(fg->pid, SIGKILL);
		    fg.job_id = 0;
		    std::cout << "smash: process " <<old<< " was killed" <<endl;
		}
	}
	else if (sig == SIGSTOP){
	    cout << "smash: caught ctrl-Z"<<endl;
	    if(fg.job_id != 0) {
	        pid_t old = fg.pid;
	        kill(fg->pid, SIGSTOP);
	        fg.seconds_elapsed = time();
	        fg.stoppped = 1;
	        jobs.push_back(fg);
	        fg.job_id = 0;
	        std::cout << "smash: process " <<old<< " was killed" << endl;
	    }
	}
	return;
}
