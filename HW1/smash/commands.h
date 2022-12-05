#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <cstdlib>
#include <sys/types.h>
#include <iostream>
#include <list>
#include <iterator>
#include <sys/wait.h>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
using namespace std;

class job {
    public:
        int job_id;
        string command;
        pid_t pid;
        time_t seconds_elapsed;
        bool stopped;
        
        job& operator=(const job& other){
        	if(this == &other){
        		return *this;
        	}
        	this->job_id = other.job_id;
        	this->command = other.command;
        	this->pid = other.pid;
        	this->seconds_elapsed = other.seconds_elapsed;
        	this->stopped = other.stopped;
        	return *this;
        }
        // maybe we need to add something with the signals
};


int ExeCmd(list<job> &jobs, char* lineSize, char* cmdString, int &quit, job& fg_cur, char* &cd, bool bg);
void ExeExternal(char *args[MAX_ARG], char* cmdString , bool bg, list<job>& jobs, job& fg_cur);
int BgCmd(char* lineSize, list<job>& jobs,  char* cmdString, int &quit, job& fg_cur, char* &cd);
#endif
