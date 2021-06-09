#pragma once
#include"headers.h"
inline void RoundRobin(const int msgq_id, int sem1, int sem2, const int timeQunatum)
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
            printf("\nat time %d end process with id = %d\n", currentTime, currentProcess.id);
            CPU_working = false;
        }
        else if (elapsedQunatum == timeQunatum && current_remain > 0)
        {
            printf("\nat time %d stop proccess with id=%d\n", currentTime, currentProcess.id, currentProcess.pid);
            kill(currentProcess.pid, SIGTSTP);
            currentProcess.runtime = current_remain;
            prevProcess = currentProcess;
            //circEnqueue(&ready_queue,&currentProcess);
            CPU_working = false;
            enQ_R_P = true;
        }
        if (!CPU_working && ready_queue.size != 0)
        {
            currentProcess=ready_queue.head->data;

            /*currentProcess.id = ready_queue.head->data.id;
            currentProcess.arrival = ready_queue.head->data.arrival;
            currentProcess.priority = ready_queue.head->data.priority;
            currentProcess.runtime = ready_queue.head->data.runtime;
            currentProcess.pid = ready_queue.head->data.pid;*/
            
            current_remain = ready_queue.head->data.runtime;
            elapsedQunatum = 0;
            kill(currentProcess.pid, SIGCONT);
            printf("\nat time %d run process with id = %d\n", currentTime, currentProcess.id);
            CPU_working = true;
            circDequeue(&ready_queue);
        }

        else if (!CPU_working && ready_queue.size == 0 && current_remain <= 0)
        {
            bool end = down_nowait(sem1);
            if (end)
            {
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
                kill(pid,SIGTSTP);
                message.p.pid = pid;
                printf("\nprocess id= %d runtime=%d", message.p.id, message.p.runtime);
                circEnqueue(&ready_queue, &message.p);
                recv = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);
            }
        }

        //if (recv == -1)
        //printf("\nat time %d Round Robin did not recieve a process \n",currentTime);

        if (enQ_R_P)
        {
            circEnqueue(&ready_queue, &prevProcess);
            enQ_R_P = false;
        }
    }
}


