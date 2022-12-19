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

struct readres_writers{
	pthread_mutex_t wrt_lock;
	pthread_mutex_t rd_lock;
	int rd_count;
	
	void init();
	void enter_reader();
	void enter_writer();
	void leave_reader();
	void leave_writer();
};

class account{
	public:
		int id;
		int password;
		int bal;
		readres_writers read_write;
	
};
