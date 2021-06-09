#include "headers.h"

void FCFS_SC(int msgq_id, int sem1, int sem2, FILE* sc_logs, FILE* sc_perf)
{
    // create ready queue
    struct List ready_queue;
    initList(&ready_queue);

    // state variables
    bool CPU_working = false;
    int prev = getClk();
    struct process current_p;
    int current_remain = 1;

    // forking variables
    int pid;

    // ipcs variables
    struct msgbuff message;
    

    // final calculations variables
    float WeightedTurnaround = 0;
    int Count = 0;
    int CPURunTime = 0;
    int WaitingTime = 0;

    while (1)
    {
        int x = getClk();

        // update state of system every one clock cycle
        if (x > prev)
        {
            prev = x;

            if (CPU_working)
            {
                current_remain--;
            }
        }

        // run a process from ready queue if we can
        if (!CPU_working && ready_queue.size != 0)
        {
            current_p.id = ready_queue.head->data.id;
            current_p.arrival = ready_queue.head->data.arrival;
            current_p.priority = ready_queue.head->data.priority;
            current_p.runtime = ready_queue.head->data.runtime;
            current_p.pid = ready_queue.head->data.pid;
            current_p.start_time = x;


            kill(current_p.pid, SIGCONT);

            current_remain = current_p.runtime;

            CPURunTime += current_p.runtime;
            WaitingTime += (x - current_p.arrival);
            Count++;

            printf("\nScheduler : at time %d run new process with id = %d at pid = %d\n", x, current_p.id, current_p.pid);
            fprintf(sc_logs, "At time %d process %d started arr %d total %d remain %d wait %d\n", x, current_p.id, current_p.arrival, current_p.runtime, current_p.runtime, x - current_p.arrival);
            CPU_working = true;
            dequeue(&ready_queue);
        }

        // end of a process
        else if (CPU_working && current_remain <= 0)
        {
            int Turnaround = (x - current_p.arrival);
            WeightedTurnaround += Turnaround / (float) current_p.runtime;
            printf("\nScheduler : at time %d end process with id = %d\n", x, current_p.id);
            fprintf(sc_logs, "At time %d process %d finished arr %d total %d remain %d wait %d\n", x, current_p.id, current_p.arrival, current_p.runtime, 0, current_p.start_time - current_p.arrival);
            CPU_working = false;
        }

        // all process in the ready queue is finished
        else if (!CPU_working && ready_queue.size == 0 && current_remain <= 0)
        {
            // check if there any process left
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

        // check for incomming process
        int rec_val = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);
        while (rec_val != -1)
        {
            printf("\nScheduler : New process arrived with id = %d\n", message.p.id);
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
                enqueue(&ready_queue, &message.p);
                kill(pid, SIGTSTP);
            }

            rec_val = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);
        }
    }
    
}

