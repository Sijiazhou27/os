#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define __USE_GNU  
#include <sched.h>  
#include "process.h"
#define my_gettime 333
#define my_printk 334


void assign_to_cpu(pid_t pid, int cpu_id){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpu_id, &mask);
    if (sched_setaffinity(pid, sizeof(mask), &mask) < 0) 
    {
    	perror("sched_setaffinity");
    }
}

void unit_forloop(){
    volatile unsigned long i;
    for(i = 0; i < 1000000UL; i ++);
    return; 
}

int set_priority(pid_t pid, int priority){
    struct sched_param param;
    param.sched_priority = priority;
    if(sched_setscheduler(pid, SCHED_FIFO, &param) < 0){
        perror("sched_setscheduler");
        return -1;
    }
    return 0; 
}


int fork_process(Process_Data process){
	//long start_t = syscall(334);
	int pid = fork();
	if(pid == 0){ 
		long start_t, end_t;
		start_t=syscall(my_gettime);
		for(int k=0;k<process.exec_time;k++)
    	{
    		unit_forloop();
    	}
		end_t = syscall(my_gettime);
    	char print_to_dmesg[100];
        sprintf(print_to_dmesg, "[Project1] %d %ld.%ld %ld.%ld\n",getpid(), start_t / 1000000000, start_t % 1000000000,
        	end_t / 1000000000, end_t % 1000000000);
        syscall(my_printk, print_to_dmesg);
		exit(0);    	
	}
	else{ 
		set_priority(pid,1);
		assign_to_cpu(pid, 1); 
		fprintf(stdout, "%s %d\n", process.name, pid);
		fflush(stdout);
		return pid;	
	}
}


void run_process(pid_t pid)
{
	set_priority(pid,80);
}

void stop_process(pid_t pid)
{
	set_priority(pid,1);
}

