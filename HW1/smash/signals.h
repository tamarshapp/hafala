#ifndef _SIGNALS_H
#define _SIGNALS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
using namespace std;
void catch_sig(int sig);


#endif

