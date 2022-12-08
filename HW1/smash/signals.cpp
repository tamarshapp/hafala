// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
//#include "commands.h"
#include <iostream>
#include <list>
using namespace std;


void catch_sig(int sig) {
	if (fg_cur.job_id==0){
		return;
	}
	//crnl c control
	if (sig == SIGINT){
		cout << "smash: caught ctrl-C"<<endl;
		//check if there is a command in fg
		if(fg_cur.job_id != 0){
			pid_t old = fg_cur.pid;
			int kill_sigit = kill(fg_cur.pid, SIGKILL);
			if(kill_sigit == -1){
				perror("smash error: kill failed");
				return;
			}
			fg_cur.job_id = 0;
			std::cout << "smash: process " <<old<< " was killed" <<endl;
		}
	}
	//crnl z control
	else if (sig == SIGTSTP){
	    cout << "smash: caught ctrl-Z"<<endl;
	    //check if there is a command in bg
	    if(fg_cur.job_id != 0) {
	        pid_t old = fg_cur.pid;
	        int kill_sigtstp = kill(fg_cur.pid, SIGSTOP);
	        if(kill_sigtstp == -1){
				perror("smash error: kill failed");
				return;
	        }
	        time(&(fg_cur.seconds_elapsed));
	        if(fg_cur.seconds_elapsed == (time_t)(-1)){
				perror("smash error: time failed");
				return;
	        }
	        fg_cur.stopped = 1;
	        if(hold_job!=0){
	        	fg_cur.job_id = hold_job;
	        }
	        else{
	        	check_list();
	        	fg_cur.job_id =max_job(jobs)+1; 	
	        }
	        hold_job = 0;
	    	list<job>::iterator it = jobs.begin();
	    	bool insert_list = false;
	    	//update hte list and inser the job to the corret place
	    	while(it != jobs.end()){
	    		if (it->job_id < fg_cur.job_id){
	    			it++;
	    		}
	    		else{
	    			insert_list = true;
	    			jobs.insert(it, fg_cur);
	    			break;
	    		}
	    	}
	    	//insert to the end of the list if its the max val
	    	if(!(insert_list)){
		        jobs.push_back(fg_cur);
	    	}
	        fg_cur.job_id = 0;
	        cout<<"smash: process " <<old<< " stopped" << endl;
	    }
	}
	return;
}
