#include <sys/types.h>

void schedule(int strategy,Process_Data *process, int process_num);

void FIFO(Process_Data *process, int process_num);

int get_next_process(Process_Data *process, int strategy, int last_idx);

void SJF(Process_Data *process, int process_num);

void PSJF(Process_Data *process, int process_num);

void RR(Process_Data *process, int process_num);