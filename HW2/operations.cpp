//		commands.c
//********************************************
#include "structs.h"
#include "operations.h"
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





// reader writers


void writer(char* args[MAX_ARGS]){
	list<account> iterator it = accounts.begin();
	//find the iterator
	
	//
	int ret = ptherad_mutex_lock(&(it->wrt_mut));
	if(ret != 0){
		perror("bank error: ptherad_mutex_lock failed");
		return;
	}
	if (!strcmp("O", args[0])){
		open_account(args);
	}
	else if(!strcmp("D", args[0])){
		depost(args);
	}
	else if(!strcmp("W", args[0])){
		withdraw(args);
	}
	else if(!strcmp("Q", args[0])){
		close_account(args);
	}
	else if(!strcmp("T", args[0])){
		trasaction(args);
	}
	else if(!strcmp("bank", args[0])){
		bank(args);
	}
	ret = ptherad_mutex_unlock(&(it->wrt_mut));
	if(ret != 0){
		perror("bank error: ptherad_mutex_unlock failed");
		return;
	}
	return;
}

void reader(char* args[MAX_ARGS]){
	
	list<account> iterator it = accounts.begin();
		//find the iterator
		
		//
	int ret = ptherad_mutex_lock(&(it->rd_mut));
	if(ret != 0){
		perror("bank error: ptherad_mutex_lock failed");
		return;
	}
	(it->rd_count)++;
	if(it->rd_count == 1){
		int ret = ptherad_mutex_lock(&(it->wrt_mut));
		if(ret != 0){
			perror("bank error: ptherad_mutex_lock failed");
			return;
		}
	}
	ret = ptherad_mutex_unlock(&(it->rd_mut));
	if(ret != 0){
		perror("bank error: ptherad_mutex_unlock failed");
		return;
	}
	do_read()
}



open_account(char* args[MAX_ARGS]){
	int check_func = check(args);
	int ret = ptherad_mutex_lock(&mut);
	if(ret != 0){
		perror("bank error: ptherad_mutex_lock failed");
		return;
	}
	
}