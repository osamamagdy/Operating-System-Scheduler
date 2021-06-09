#include "headers.h"

void SRTN_SC(int msgq_id, int sem1, int sem2, FILE* sc_logs, FILE* sc_perf, int capacity)
{
    struct Heap *ready_queue = CreateHeap(capacity);
    bool CPU_working = false;
    int pid;
    struct msgbuff message;
    int prev = getClk();
    struct process *current_p;
    int current_remain = INT_MAX;

    float WeightedTurnaround = 0;
    int Count = 0;
    int CPURunTime = 0;
    int WaitingTime = 0;


    while (1)
    {

        int x = getClk();
        if (x > prev)
        {
            prev = x;

            if (CPU_working)
            {
                current_remain--;
            }
        }
        if (!CPU_working && ready_queue->count != 0)
        {
            current_p = PopMin(ready_queue);
            kill(current_p->pid, SIGCONT);
            CPU_working = true;
            current_remain = current_p->runtime;

            if (current_p->runtime == current_p->runtime_log)
            {
                current_p->start_time = x;
                current_p->last_run = x;
                current_p->total_wait = x - current_p->arrival;
                CPURunTime += current_p->runtime;
                Count++;
                printf("Scheduler : at time %d run new process with id = %d at pid = %d\n", x, current_p->id, current_p->pid);
                fprintf(sc_logs, "At time %d process %d started arr %d total %d remain %d wait %d\n", x, current_p->id, current_p->arrival, current_p->runtime_log, current_p->runtime, current_p->total_wait);
               
            }
            else
            {
                
                current_p->total_wait += (x - current_p->last_run);
                printf("Scheduler : at time %d re_run new process with id = %d at pid = %d\n", x, current_p->id, current_p->pid);
                fprintf(sc_logs, "At time %d process %d resumed arr %d total %d remain %d wait %d\n", x, current_p->id, current_p->arrival, current_p->runtime_log, current_p->runtime, current_p->total_wait);
               
            }

            
        }
        else if (CPU_working && current_remain <= 0)
        {
            int Turnaround = (x - current_p->arrival);
            WeightedTurnaround += Turnaround / (float) current_p->runtime_log;
            WaitingTime += current_p->total_wait;
            fprintf(sc_logs, "At time %d process %d finished arr %d total %d remain %d wait %d\n", x, current_p->id, current_p->arrival,  current_p->runtime_log, 0,  current_p->total_wait);
            free(current_p);
            CPU_working = false;
        }
        else if (!CPU_working && ready_queue->count == 0 && current_remain <= 0)
        {

            bool end = down_nowait(sem1);
            if (end)
            {
                int TotalCPUTime = x;
                fprintf(sc_perf, "CPU utilization = %.2f%\n", (CPURunTime / (float)TotalCPUTime)*100);
                fprintf(sc_perf, "Avg WTA = %.2f\n",  WeightedTurnaround / (float)Count);
                fprintf(sc_perf, "Avg Waiting = %.2f\n", WaitingTime / (float)Count);
                
                up(sem2);
                return;
            }
        }

        int rec_val = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);

        while (rec_val != -1)
        {

            printf("\nScheduler : At time %d New process arrived with id = %d\n", x, message.p.id);
            pid = fork();
            if (pid == 0)
            {
                printf("Forked\n");
                char snum[5];
                sprintf(snum, "%d", message.p.runtime);
                char *args[] = {"./process.out", snum, NULL};
                if (execv(args[0], args) < 0)
                    printf("Failed\n");
            }
            else
            {

                message.p.pid = pid;
                message.p.priority = message.p.runtime;
                struct process *temp_p = (struct process *)malloc(sizeof(struct process));
                *temp_p = message.p;
                kill(pid, SIGTSTP);
                insert(ready_queue, temp_p);

                if (current_remain != INT_MAX && current_remain > temp_p->priority)
                {
                    printf("process %d is preempted\n", current_p->id);
                    fprintf(sc_logs, "At time %d process %d stopped arr %d total %d remain %d wait %d\n", x, current_p->id, current_p->arrival, current_p->runtime_log, current_remain, current_p->total_wait);
                    kill(current_p->pid, SIGTSTP);
                    current_p->runtime = current_remain;
                    current_p->priority = current_remain;
                    current_p->last_run = x;
                    insert(ready_queue, current_p);
                    current_remain = 0;
                    CPU_working = false;
                }
                print(ready_queue);
            }

            rec_val = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);
        }
    }
}