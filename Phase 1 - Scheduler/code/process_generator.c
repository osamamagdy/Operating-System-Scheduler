#include "headers.h"
#include "list.c"
void clearResources(int);



int main(int argc, char *argv[])
{

    /**
     * @todo We need to make this the number of processes in the file
     * 
     */
    int p_num = 0;

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

    //char * file_name = argv[1];   //This line in case we want to pass the file name as an arguement but it's not mentioned in the documentation.
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

        enqueue(&array, p);

        fscanf(fp, "%d", &id);
        p_num++;
    }

    printf("%d\n", p_num);

    //Close the file
    fclose(fp);

    /**
     * @brief Here we read the chosen algorithm and switch according to #defines in the header file
     * @todo modify if the algorithm needs some parameters that is expected to be provided by the command line
     * @todo Write the switch case for each algorithm
     */
    char *algo = argv[1];

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
                char *args[] = {"./scheduler.out", NULL};
                execv(args[0], args);
            }
            /* code */
        }
        else if (pid == -1)
        {
            printf("Error\n");
        }
    }

    printf("I am the parent and my childs pid = %d and mine is %d\n", pid, getpid());

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
                printf("Process %d arrived at time : %d\n",array.head->data.id, x);
                dequeue(&array);
                process_pointer++;
                if (process_pointer == p_num)
                {
                    break;
                    /* code */
                }
            }
        }
        if (process_pointer == p_num)
        {
            break;
            /* code */
        }
    }

    /**
     * @todo Upon exit we might need to call clearResources instead of only destroyClk
     * 
     */

    destroyClk(1);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    destroyClk(1);
    raise(SIGKILL);
}
