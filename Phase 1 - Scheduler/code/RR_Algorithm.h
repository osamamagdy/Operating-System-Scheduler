#include "headers.h"

void circEnqueue(struct List *l, struct process *p);
void circDequeue(struct List *l);
void RoundRobin(const int msgq_id, int sem1, int sem2, const int timeQunatum, FILE* sc_logs, FILE* sc_perf)
{
    struct List ready_queue;
    initList(&ready_queue);

    struct msgbuff message;
    struct process currentProcess;
    struct process prevProcess;
    bool enQ_R_P = false;
    bool CPU_working = false;
    int pid;
    int current_remain = 1;
    int elapsedQunatum = 0;

    float WeightedTurnaround = 0;
    int Count = 0;
    int CPURunTime = 0;
    int WaitingTime = 0;

    struct Node *headePtr = NULL;

    int prevTime = getClk();
    while (1)
    {
        int currentTime = getClk();
        if (currentTime > prevTime)
        {
            prevTime = currentTime;

            if (CPU_working)
            {
                current_remain--;
                elapsedQunatum++;
            }
        }
        if (CPU_working && current_remain <= 0)
        {
            int Turnaround = (currentTime - currentProcess.arrival);
            WeightedTurnaround += Turnaround / (float) currentProcess.runtime_log;
            WaitingTime += currentProcess.total_wait;
            fprintf(sc_logs, "At time %d process %d finished arr %d total %d remain %d wait %d\n", currentTime, currentProcess.id, currentProcess.arrival,  currentProcess.runtime_log, 0,  currentProcess.total_wait);
            CPU_working = false;
        }
        else if (elapsedQunatum == timeQunatum && current_remain > 0)
        {
            if (ready_queue.size == 0)
                elapsedQunatum = 0;
            else
            {
                printf("process %d is preempted\n", currentProcess.id);
                fprintf(sc_logs, "At time %d process %d stopped arr %d total %d remain %d wait %d\n", currentTime, currentProcess.id, currentProcess.arrival, currentProcess.runtime_log, current_remain, currentProcess.total_wait);
                kill(currentProcess.pid, SIGTSTP);
        
                currentProcess.last_run = currentTime;
                currentProcess.runtime = current_remain;
                prevProcess = currentProcess;
                CPU_working = false;
                enQ_R_P = true;
            }
        }
            
        if (!CPU_working && ready_queue.size != 0)
        {
            currentProcess = ready_queue.head->data;

            
            current_remain = ready_queue.head->data.runtime;
            elapsedQunatum = 0;
            kill(currentProcess.pid, SIGCONT);
            CPU_working = true;
            circDequeue(&ready_queue);

             if (currentProcess.runtime == currentProcess.runtime_log)
            {
                currentProcess.start_time = currentTime;
                currentProcess.last_run = currentTime;
                currentProcess.total_wait = currentTime - currentProcess.arrival;
                CPURunTime += currentProcess.runtime;
                Count++;
                printf("Scheduler : at time %d run new process with id = %d at pid = %d\n", currentTime, currentProcess.id, currentProcess.pid);
                fprintf(sc_logs, "At time %d process %d started arr %d total %d remain %d wait %d\n", currentTime, currentProcess.id, currentProcess.arrival, currentProcess.runtime_log, currentProcess.runtime, currentProcess.total_wait);
               
            }
            else
            {
                
                currentProcess.total_wait += (currentTime - currentProcess.last_run);
                
                printf("Scheduler : at time %d re_run new process with id = %d at pid = %d\n", currentTime, currentProcess.id, currentProcess.pid);
                fprintf(sc_logs, "At time %d process %d resumed arr %d total %d remain %d wait %d\n", currentTime, currentProcess.id,currentProcess.arrival, currentProcess.runtime_log, currentProcess.runtime, currentProcess.total_wait);
               
            }

        
        }

        else if (!CPU_working && ready_queue.size == 0 && current_remain <= 0)
        {
            bool end = down_nowait(sem1);
            if (end)
            {
                int TotalCPUTime = currentTime;
                fprintf(sc_perf, "CPU utilization = %.2f%\n", (CPURunTime / (float)TotalCPUTime)*100);
                fprintf(sc_perf, "Avg WTA = %.2f\n",  WeightedTurnaround / (float)Count);
                fprintf(sc_perf, "Avg Waiting = %.2f\n", WaitingTime / (float)Count);
                
                up(sem2);
                return;
            }
        }

        int recv = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);
        while (recv != -1)
        {
            pid = fork();
            if (pid == 0)
            {
                char snum[5];
                sprintf(snum, "%d", message.p.runtime);
                char *args[] = {"./process.out", snum, NULL};
                execv(args[0], args);
            }
            else
            {
                kill(pid, SIGTSTP);
                message.p.pid = pid;
                //printf("\nprocess id= %d runtime=%d", message.p.id, message.p.runtime);
                circEnqueue(&ready_queue, &message.p);
                recv = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);
            }
        }

        //if (recv == -1)
        //printf("\nat time %d Round Robin did not recieve a process \n",currentTime);

        if (enQ_R_P)
        {
            enQ_R_P = false;
            circEnqueue(&ready_queue, &prevProcess);
            //printf("\nat time %d enqueue process with id= %d , readyQ size= %d\n", currentTime, prevProcess.id, ready_queue.size);
            elapsedQunatum=0;
        }
    }
}

void circEnqueue(struct List *l, struct process *p)
{
    struct Node *n = (struct Node *)malloc(sizeof(struct Node));
    n->data = *p;
    n->next = NULL;

    if (l->size == 0)
    {
        l->head = n;
        l->tail = n;
        n->next = l->head; // l->tail->next=l->head;
    }
    else
    {
        l->tail->next = n;
        l->tail = n;
        n->next = l->head; // l->tail->next = l->head;
    }
    l->size++;
}
void circDequeue(struct List *l)
{
    if (l->size == 0)
        return;

    struct Node *dNode = l->head;
    l->head = l->head->next;
    l->tail->next = l->head;
    free(dNode);
    dNode = NULL;

    l->size--;
}

