//		commands.c
//********************************************
#include "structs.h"
#include <semaphore.h>
#include <pthreads.h>
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


// readers writers
pthread_mutex_t wrt_lock;
pthread_mutex_t rd_lock;
int rd_count;

