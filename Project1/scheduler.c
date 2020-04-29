#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "process.h"
#include "scheduler.h"
// #define FIFO 0
// #define SJF 1
// #define PSJF 2
// #define RR 3
int start_num = 0;
int finish_num = 0;
void schedule(int strategy,Process_Data *process, int process_num)
{
	if(strategy==0)
	{
		FIFO(process,process_num);
	}
	else if(strategy==1)
	{
		SJF(process,process_num);
	}
	else if(strategy==2)
	{
		PSJF(process,process_num);
	}
	else if(strategy==3)
	{
		RR(process,process_num);
	}
}

int get_next_process(Process_Data *process, int strategy, int last_index)
{
	if(strategy == 0)//FIFO
	{
		for(int i = 0; i < start_num; i++)
		{
			if(process[i].exec_time > 0)
			{
				return i;
			}
		}
	}
	if(strategy == 1 || strategy == 2)// SJF PSJF
	{
		int min_time = 2147483647, min_idx = -1;
		for(int i = 0; i < start_num; i++)
		{
			if(process[i].exec_time > 0)
			{
				if(process[i].exec_time < min_time)
				{
					min_time = process[i].exec_time;
					min_idx = i;
				}	
			}
		}
		return min_idx;
	}
	if(strategy==3)
	{
		int next_index;
		next_index = (last_index + 1) % start_num;
		for(int i = 0; i < start_num; i++)
		{
			if(process[next_index].exec_time > 0)
			{
				return next_index;
			}
			next_index = (next_index + 1) % start_num;
		}
	}
	return -1;
}

void FIFO(Process_Data *process, int process_num)
{
	assign_to_cpu(getpid(),0);
	set_priority(getpid(),80);
	int time_now=0;
	int running_index=-1;
	int finish_time=-1;
	while(1)
	{
		if(finish_num==process_num)
		{
			break;
		}
		for(int i=0;i<process_num;i++)
		{
			if(process[i].start_time==time_now)
			{
				start_num++;
				process[i].pid=fork_process(process[i]);
			}
		}
		if(finish_num < start_num && finish_time <= time_now)
		{
			running_index = get_next_process(process, 0, running_index);
			if(running_index >= 0)
			{
				run_process(process[running_index].pid);
				finish_time = time_now + process[running_index].exec_time;
			}
		}
		unit_forloop();
		time_now ++;
		if(running_index >= 0)
		{
			process[running_index].exec_time --;
			if(process[running_index].exec_time == 0)
			{			
				finish_num ++;
			}
		}

	}
	int status;
	for(int i = 0; i < process_num; i++)
	{
		wait(&status);
	}
}

void SJF(Process_Data *process, int process_num)
{
	assign_to_cpu(getpid(),0);
	set_priority(getpid(),80);
	int time_now=0;
	int running_index=-1;
	int finish_time=-1;
	while(1)
	{
		if(finish_num==process_num)
		{
			break;
		}
		for(int i=0;i<process_num;i++)
		{
			if(process[i].start_time==time_now)
			{
				start_num++;
				process[i].pid=fork_process(process[i]);
			}
		}
		if(finish_num < start_num && finish_time <= time_now)
		{
			running_index = get_next_process(process,1, running_index);
			if(running_index >= 0)
			{
				run_process(process[running_index].pid);
				finish_time = time_now + process[running_index].exec_time;
			}
		}
		unit_forloop();
		time_now ++;
		if(running_index >= 0)
		{
			process[running_index].exec_time --;
			if(process[running_index].exec_time == 0)
			{		
				finish_num ++;
			}
		}

	}
	int status;
	for(int i = 0; i < process_num; i++)
	{
		wait(&status);
	}
}

void PSJF(Process_Data *process, int process_num)
{
	assign_to_cpu(getpid(),0);
	set_priority(getpid(),80);
	int time_now=0;
	int running_index=-1;
	while(1)
	{
		if(finish_num==process_num)
		{
			break;
		}
		for(int i=0;i<process_num;i++)
		{
			if(process[i].start_time==time_now)
			{
				start_num++;
				process[i].pid=fork_process(process[i]);
			}
		}
		if(finish_num < start_num)
		{
			running_index = get_next_process(process,2, running_index);
			if(running_index >= 0)
			{
				run_process(process[running_index].pid);
			}
		}
		unit_forloop();
		time_now ++;
		if(running_index >= 0)
		{
			process[running_index].exec_time --;
			if(process[running_index].exec_time == 0)
			{				
				finish_num ++;
			}
			if(process[running_index].exec_time > 0)
				stop_process(process[running_index].pid);
		}

	}
	int status;
	for(int i = 0; i < process_num; i++)
	{
		wait(&status);
	}
}

void RR(Process_Data *process, int process_num)
{
	assign_to_cpu(getpid(),0);
	set_priority(getpid(),80);
	int time_now=0;
	int running_index=-1;
	int finish_time=-1;
	while(1)
	{
		if(finish_num==process_num)
		{
			break;
		}
		for(int i=0;i<process_num;i++)
		{
			if(process[i].start_time==time_now)
			{
				start_num++;
				process[i].pid=fork_process(process[i]);
			}
		}
		if(finish_num < start_num && finish_time <= time_now)
		{
			running_index = get_next_process(process,3, running_index);
			if(running_index >= 0)
			{
				run_process(process[running_index].pid);
				if(process[running_index].exec_time<=500)
					finish_time = time_now + process[running_index].exec_time;
				else
				{
					finish_time = time_now + 500;
				}
			}
		}
		unit_forloop();
		time_now ++;
		if(running_index >= 0)
		{
			process[running_index].exec_time --;
			if(process[running_index].exec_time == 0)
			{			
				finish_num ++;
			}
			if (finish_time==time_now)
				if(process[running_index].exec_time > 0)
					stop_process(process[running_index].pid);
		}

	}
	int status;
	for(int i = 0; i < process_num; i++)
	{
		wait(&status);
	}
}
	


