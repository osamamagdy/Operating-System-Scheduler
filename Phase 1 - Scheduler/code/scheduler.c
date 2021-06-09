#include "FCFS_Algorithm.h"
#include "SJF_Algorithm.h"
#include "HPF_Algorithm.h"
#include "SRTN_Algorithm.h"
#include "RR_Algorithm.h"

void clearResources(int signum);
int process_shmid;
int msgq_id,sem1,sem2;
union Semun semun;

int main(int argc, char *argv[])
{

    initClk();
    printf("%s", argv[1]);

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


    FILE* sc_logs = fopen("scheduler.log", "w");
    FILE* sc_perf = fopen("Scheduler.perf", "w");

    if (sc_logs == NULL || sc_perf == NULL)
    {
        printf("Couldn't open  for logs\n");
        return 0;
    }

    fprintf(sc_logs, "#At time x process y state arr w total z remain y wait k\n");

    if (strcmp(argv[1], "1") == 0)
        FCFS_SC(msgq_id, sem1, sem2, sc_logs, sc_perf);
    else if (strcmp(argv[1], "2") == 0)
        SJF_SC(msgq_id, sem1, sem2, sc_logs, sc_perf, atoi(argv[2]));
    else if (strcmp(argv[1], "3") == 0)
        HPF_SC(msgq_id, sem1, sem2, sc_logs, sc_perf, atoi(argv[2]));
    else if (strcmp(argv[1], "4") == 0)
        SRTN_SC(msgq_id, sem1, sem2, sc_logs, sc_perf, atoi(argv[2]));
    else 
        RoundRobin(msgq_id, sem1, sem2, atoi(argv[3]), sc_logs, sc_perf);
    fclose(sc_logs);
    fclose(sc_perf);

    clearResources(0);

    printf("Scheduler ended\n");

    /**
     * @todo Remove this Sleep
     * 
     */
    sleep(50);
    destroyClk(true);
}


void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    destroyClk(1);
    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *) 0);
    semctl(sem1,0 , IPC_RMID,semun);
    semctl(sem2,0 , IPC_RMID,semun);
    raise(SIGKILL);
}
