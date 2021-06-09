#include "headers.h"

int process_shmid;




void clearResources(int signum);

int main(int argc, char *argv[])
{

    initClk();

    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.

    key_t key_id;
    int msgq_id;
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

    SJF_SC(msgq_id, sem1, sem2);

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

    ////To use with Processes 
    key_t process_shmkey_id ;
    process_shmkey_id = ftok("keyfile", 'M');                  //create a key with the id
    process_shmid = shmget(process_shmkey_id, 256, 0666 | IPC_CREAT); //With IPC_CREAT as a flag--> this msgget will search for a shared memory with the id of the key_id (65)

    if (process_shmid == -1)
    {
        perror("Error in create");
        exit(-1);
    }
//    printf("shared memory ID = %d\n", process_shmid);

    void *process_shmaddr = shmat(process_shmid, (void *)0, 0);
    if (process_shmaddr == -1)
    {
        perror("Error in attach in reader");
        exit(-1);
    }
    else
    {
//        printf("\nReader: Shared memory attached at address %x\n", shmaddr);

    }

    process_shmaddr = &(curent_p.pid);

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

//            printf("shared memory with %d\n",*((int*)process_shmaddr));

            kill(curent_p.pid, SIGCONT);

            current_remain = curent_p.runtime;
            printf("Scheduler : at time %d run new process with id = %d at pid = %d\n", x, curent_p.id, curent_p.pid);
            CPU_working = true;
            dequeue(&ready_queue);
        }
        else if (CPU_working && current_remain <= 0)
        {
            printf(" Scheduler : at time %d end process with id = %d\n", x, curent_p.id);
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
            printf("Scheduler : New process arrived with id = %d\n", message.p.id);
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
                enqueue(&ready_queue, &message.p);
//                printf("pid is %d == %d\n", pid, message.p.pid);
                kill(pid, SIGTSTP);
            }
        }
    }
    clearResources(0);
}

void SJF_SC(int msgq_id, int sem1, int sem2)
{
    struct Heap * ready_queue = CreateHeap(100);

    bool CPU_working = false;
    int pid;
    struct msgbuff message;
    int prev = getClk();
    struct process * curent_p;
    int current_remain = 1;

    ////To use with Processes 
    key_t process_shmkey_id ;
    process_shmkey_id = ftok("keyfile", 'M');                  //create a key with the id
    process_shmid = shmget(process_shmkey_id, 256, 0666 | IPC_CREAT); //With IPC_CREAT as a flag--> this msgget will search for a shared memory with the id of the key_id (65)

    if (process_shmid == -1)
    {
        perror("Error in create");
        exit(-1);
    }
//    printf("shared memory ID = %d\n", process_shmid);

    void *process_shmaddr = shmat(process_shmid, (void *)0, 0);
    if (process_shmaddr == -1)
    {
        perror("Error in attach in reader");
        exit(-1);
    }
    else
    {
//        printf("\nReader: Shared memory attached at address %x\n", shmaddr);

    }

    process_shmaddr = &(curent_p->pid);

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
            
//          printf("shared memory with %d\n",*((int*)process_shmaddr));

            kill(curent_p->pid, SIGCONT);

            current_remain = curent_p->runtime;
            printf("Scheduler : at time %d run new process with id = %d at pid = %d\n", x, curent_p->id, curent_p->pid);
            CPU_working = true;

        }
        else if (CPU_working && current_remain <= 0)
        {
            printf(" Scheduler : at time %d end process with id = %d\n", x, curent_p->id);
            //free(curent_p);
            CPU_working = false;
        }
        else if (!CPU_working && ready_queue->count == 0 && current_remain <= 0)
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
            printf("Scheduler : New process arrived with id = %d\n", message.p.id);
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
                insert(ready_queue, &message.p);
//                printf("pid is %d == %d\n", pid, message.p.pid);
                kill(pid, SIGTSTP);
            }
        }
    }
    clearResources(0);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    shmctl(process_shmid, IPC_RMID, (struct shmid_ds *)0);
    raise(SIGKILL);
}
