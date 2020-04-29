#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"
int main(int argc, char const *argv[])
{
	char strategy[20];
	int process_num;
	scanf("%s", strategy);
	scanf("%d", &process_num);
	Process_Data *process;
	process = malloc(process_num*sizeof(Process_Data));
	for(int i=0;i<process_num;i++)
	{
		scanf("%s %d %d",process[i].name,&process[i].start_time,&process[i].exec_time);
		process[i].pid=-1;
	}
    int strate;
    if(strcmp(strategy, "FIFO") == 0)
        strate = 0;
    else if(strcmp(strategy, "SJF") == 0)
        strate = 1;
    else if(strcmp(strategy, "PSJF") == 0)
        strate = 2;
    else if(strcmp(strategy, "RR") == 0)
        strate = 3;
    else{
        printf("Invalid strategy");
        return 0;
	}
	schedule(strate,process,process_num);
	free(process);
	return 0;
}