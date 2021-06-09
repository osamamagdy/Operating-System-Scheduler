#include "headers.h"
void SJF_SC(int msgq_id, int sem1, int sem2,  FILE* sc_logs, FILE* sc_perf, int capacity)
{
    struct Heap * ready_queue = CreateHeap(capacity);

    bool CPU_working = false;
    int pid;
    struct msgbuff message;
    int prev = getClk();
    struct process * curent_p;
    int current_remain = 1;

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
            curent_p= PopMin(ready_queue);
            curent_p->start_time = x;

            kill(curent_p->pid, SIGCONT);

            CPURunTime += curent_p->runtime;
            WaitingTime += (x - curent_p->arrival);
            Count++;

            current_remain = curent_p->runtime;
            printf("\nScheduler : at time %d run new process with id = %d at pid = %d\n", x, curent_p->id, curent_p->pid);
            fprintf(sc_logs, "At time %d process %d started arr %d total %d remain %d wait %d\n", x, curent_p->id, curent_p->arrival, curent_p->runtime, curent_p->runtime, x - curent_p->arrival);
            CPU_working = true;

        }
        else if (CPU_working && current_remain <= 0)
        {
            int Turnaround = (x - curent_p->arrival);
            WeightedTurnaround += Turnaround / (float) curent_p->runtime;
            printf("\nScheduler : at time %d end process with id = %d\n", x, curent_p->id);
            fprintf(sc_logs, "At time %d process %d finished arr %d total %d remain %d wait %d\n", x, curent_p->id, curent_p->arrival,  curent_p->runtime, 0,curent_p->start_time - curent_p->arrival);
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

            printf("Scheduler : New process arrived with id = %d\n", message.p.id);
            pid = fork();
            if (pid == 0)
            {
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
                struct process * temp_p = (struct process *)malloc(sizeof(struct process));
                *temp_p = message.p; 
                insert(ready_queue, temp_p);
                printf("pid is %d == %d\n", pid, message.p.pid);
                
                print(ready_queue);

                kill(pid, SIGTSTP);
            }
            rec_val = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);

        }
    }
 
}