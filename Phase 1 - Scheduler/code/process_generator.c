#include "headers.h"
void clearResources(int);

int process_shmid;
int msgq_id,sem1,sem2;
union Semun semun;


int main(int argc, char *argv[])
{

    
    int p_num = 0;
    semun.val =0;
    key_t key_id;
    int send_val;

    key_id = ftok("keyfile", 65);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);

    sem1 = semget(key_id, 1, 0666 | IPC_CREAT);
    sem2 = semget(key_id + 1, 1, 0666 | IPC_CREAT);
    if (sem1 == -1 || sem2 == -1)
    {
        perror("Error in create sem");
        exit(-1);
    }

    union Semun semun;
    semun.val = 0; 
    if (semctl(sem1, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }
    if (semctl(sem2, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }


    if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }

    /**
     * @brief Construct a new signal object and bind the ^C to clear resources upon completion.
     * @todo We may need to call clearResources at the end of tht main
     * 
     */
    signal(SIGINT, clearResources);

    // 1. Read the input files.
    /**
     * @brief Read the input file, ignore the first four words, and Create array of pointers to struct process
     * @todo IF we will read the name of the file from command line arguements , 
     * we need to uncomment the line below, comment the line after it, modify the variable where we read the algo, and modify the make file accordingly.
     * 
     */

    char *file_name = "processes.txt";

    FILE *fp = fopen(file_name, "r");

    if (fp == NULL)
    {
        printf("Couldn't open %s for reading\n", file_name);
        return 0;
    }

    int id, arrival, runtime, priority;
    char *word;
    fscanf(fp, "%s", &word);
    fscanf(fp, "%s", &word);
    fscanf(fp, "%s", &word);
    fscanf(fp, "%s", &word);

    // //Create array of pointers to processes
    struct List array;
    initList(&array); 

    fscanf(fp, "%d", &id);
    while (!feof(fp))
    {
        fscanf(fp, "%d %d %d" ,&arrival, &runtime, &priority);
        struct process *p = malloc(sizeof(struct process));
        p->id = id;
        p->arrival = arrival;
        p->runtime = runtime;
        p->priority = priority;
        p->runtime_log = runtime;

        enqueue(&array, p);

        fscanf(fp, "%d", &id);
        p_num++;
    }


    //Close the file
    fclose(fp);

    printf("\nSimulation Start....\n");
  

    /**
     * @brief generate the clock & scheduler processes according to their names in the make file
     */
    int pid;
    for (int i = 0; i < 2; i++)
    {

        pid = fork();
        if (pid == 0)
        {
            if (i == 0)
            {
                char *args[] = {"./clk.out", NULL};
                execv(args[0], args);
            }
            else if (i == 1)
            {
                
                char snum[10];
                sprintf(snum, "%d", p_num);
                char *args[] = {"./scheduler.out", argv[1], snum, argv[2],NULL};
                execv(args[0], args);           
            
            }

        }
        else if (pid == -1)
        {
            printf("Error\n");
        }
    }

    /**
     * @brief Construct a new init Clk object
     * Use this function inside every process deals with the clock after creating the clock process to initialize clock. 
     * It is blocking function untill the process of the clock is created
     */
    initClk();

    /**
     * @brief The main loop function that at each time step generates a process (like we did for clock and scheduler)
     * And send its info to the Scheduler via message queue 
     * @todo Generate process and send info
     * 
     */

    // To get time use this function.
    int prev = getClk();
    int process_pointer = 0;
    while (1)
    {
        int x = getClk();

        if (x > prev)
        {

            prev = x;
            while (array.head->data.arrival == x)
            {
                struct msgbuff message;
                message.mtype = 7; 
                message.p = array.head->data;
                send_val = msgsnd(msgq_id, &message, sizeof(message.p), !IPC_NOWAIT);
                if (send_val == -1)
                    perror("Errror in send");
                dequeue(&array);
            
                process_pointer++;
                if (process_pointer == p_num)
                {
                    break;
                }
            }
        }
        if (process_pointer == p_num)
        {
            break;
        }
    }

    /**
     * @brief Upon exit we might need to call clearResources instead of only destroyClk
     * 
     */
    
    up(sem1);
    down(sem2);

    clear_sem(sem1);
    clear_sem(sem2);

    printf("\nSimulation end...\n");
    clearResources(0);
}

void clearResources(int signum)
{
    
    shmctl(process_shmid, IPC_RMID, (struct shmid_ds *)0);
    raise(SIGKILL);
}

