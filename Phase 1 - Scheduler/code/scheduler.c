#include "headers.h"

void clearResources(int signum);



int main(int argc, char *argv[])
{
    
    initClk();


    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.

    key_t key_id;
    int  msgq_id;
    signal(SIGINT, clearResources);


    key_id = ftok("keyfile", 65);               //create unique key
    msgq_id = msgget(key_id, 0666 | IPC_CREAT); //create message queue and return id
    int sem1 = semget(key_id, 1, 0666 | IPC_CREAT);
    int sem2 = semget(key_id + 1, 1, 0666 | IPC_CREAT);

    if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }

    FCFS_SC(msgq_id, sem1, sem2);

    printf("Scheduler ended\n");

    /**
     * @todo Remove this Sleep
     * 
     */
    sleep(50);
    destroyClk(true);
}

void FCFS_SC(int msgq_id, int sem1, int sem2)
{
    struct List ready_queue;
    initList(&ready_queue);
    bool CPU_working = false;
    int pid;
    struct msgbuff message;
    int prev = getClk();
    struct process curent_p;
    int current_remain = 1;

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

        if (!CPU_working && ready_queue.size != 0)
        {
            curent_p.id = ready_queue.head->data.id;
            curent_p.arrival = ready_queue.head->data.arrival;
            curent_p.priority = ready_queue.head->data.priority;
            curent_p.runtime = ready_queue.head->data.runtime;
            curent_p.pid = ready_queue.head->data.pid;

            
            //kill(curent_p.pid, SIGUSR2);

            current_remain = curent_p.runtime;
            printf("at time %d run new process with id = %d at pid = %d\n", x, curent_p.id,curent_p.pid);
            CPU_working = true;
            dequeue(&ready_queue);
        }
        else if(CPU_working && current_remain <= 0)
        {
            printf("at time %d end process with id = %d\n", x, curent_p.id);   
            CPU_working = false;
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

        int rec_val = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);
        if (rec_val != -1)
        {
            printf("New process arrived with id = %d\n", message.p.id);
            enqueue(&ready_queue, &message.p);
            pid = fork();
            if (pid == 0)
            {
                printf("Forked\n");
                char snum[5];
                sprintf(snum, "%d", message.p.runtime);
                char *args[] = {"./process.out", snum, NULL};
                execv(args[0], args);
            }
            ready_queue.head->data.pid = pid;
            printf("pid is %d == %d\n", pid,ready_queue.head->data.pid);
            //kill(pid,SIGUSR1);

        }
    }
    
}


void SJF_SC(int msgq_id, int sem1, int sem2)
{
    struct Heap * ready_queue = CreateHeap(100);
    printf("Created heap with count %d, capacity %d \n", ready_queue->count, ready_queue->capacity);
    bool CPU_working = false;
    int pid;
    struct msgbuff message;
    int prev = getClk();
    struct process * curent_p;
    int current_remain = 1;

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
            printf("current time %d\n",x);
        }

        if (!CPU_working && ready_queue->count > 0)
        {
            printf("at time %d hi 1",x);
            curent_p = PopMin(ready_queue);

            current_remain = curent_p->runtime;

            printf("at time %d run new process with id = %d\n", x, curent_p->id);
            CPU_working = true;

        }
        else if(CPU_working && current_remain <= 0)
        {
            printf("at time %d end process with id = %d\n", x, curent_p->id);
            CPU_working = false;
        }
        else if (!CPU_working && ready_queue->capacity == 0 && current_remain <= 0)
        {
            printf("at time %d hi 2",x);
            bool end = down_nowait(sem1);
            if (end)
            {
                up(sem2);
                return;
            }
       
        }

        int rec_val = msgrcv(msgq_id, &message, sizeof(message.p), 0, IPC_NOWAIT);

        if (rec_val != -1)
        {
            printf("at time %d New process arrived with id = %d\n", x, message.p.id);
            curent_p->priority = curent_p->runtime;
        
            //insert(ready_queue , curent_p);
            printf("at time %d hi 1",x);

            pid = fork();
            if (pid == 0)
            {
                char snum[5];
                sprintf(snum, "%d", message.p.runtime);
                char *args[] = {"./process.out", snum, NULL};
                execv(args[0], args);
            }
        }
    }
    
    /*
    Heap PQ();
    int currRemaining = 0; 
    while(!empty(PQ) && !empty(array))
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

        if (!CPU_working && ready_queue.size != 0)
        {
            curent_p.id = ready_queue.head->data.id;
            curent_p.arrival = ready_queue.head->data.arrival;
            curent_p.priority = ready_queue.head->data.priority;
            curent_p.runtime = ready_queue.head->data.runtime;
            current_remain = ready_queue.head->data.runtime;

            printf("at time %d run new process with id = %d\n", x, curent_p.id);
            CPU_working = true;
            dequeue(&ready_queue);
        }
        else if(CPU_working && current_remain <= 0)
        {
            printf("at time %d end process with id = %d\n", x, curent_p.id);   
            CPU_working = false;
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


        if(process arrived)
            PQ.push(proccess);
        if(currRemaining) 
            currRemaining--;
        else

        {
            process = PQ.pop();
            currRemaining = process.runtime;
            start(process);
        }
    }

    */

}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    raise(SIGKILL);
}
