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
#include <string>
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

extern int hold_job;
extern job fg_cur;
extern list<job> jobs;
extern string cd1;
int ExeCmd( char* lineSize, char* cmdString, int &quit,  bool bg);
void ExeExternal(char *args[MAX_ARG], char* cmdString , bool bg);
int BgCmd(char* lineSize,  char* cmdString, int &quit);
int max_job(list<job> jobs);
void check_list();

#endif
