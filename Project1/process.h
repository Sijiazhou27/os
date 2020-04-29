#include <sys/types.h>

typedef struct {
		char name[10];
		int start_time;
		int exec_time;
		pid_t pid;
}Process_Data;

void assign_to_cpu(pid_t pid, int cpu_id);

void unit_forloop();

int set_priority(pid_t pid, int priority);

int fork_process(Process_Data process);

void run_process(pid_t pid);

void stop_process(pid_t pid);