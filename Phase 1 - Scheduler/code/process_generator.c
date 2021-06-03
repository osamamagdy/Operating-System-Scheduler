#include "headers.h"
void clearResources(int);





struct process
{
    int id,arrival,runtime,priority;
    /* data */
};



int main(int argc, char *argv[])
{

    int p_num = 10;
    signal(SIGINT, clearResources);
    // TODO Initialization


    // 1. Read the input files.
    //char * file_name = argv[1];   //This line in case we want to pass the file name as an arguement but it's not mentioned in the documentation.
    char * file_name = "processes.txt";


    FILE *fp = fopen(file_name, "r");


    if (fp==NULL)
    {
        printf("Couldn't open %s for reading\n",file_name);
        return 0;
    }

    int id,arrival,runtime,priority;
    char * word;
    fscanf(fp, "%s",&word);
    fscanf(fp, "%s",&word);
    fscanf(fp, "%s",&word);
    fscanf(fp, "%s",&word);

    // //Create array of pointers to processes 
    struct process ** array = (struct process **)malloc(p_num * sizeof(struct process *));

    for (int i = 0; i < 10; i++)
    {
        fscanf(fp,"%d %d %d %d",&id , &arrival , &runtime , &priority);
        array[i] = malloc( sizeof(struct process));
        array[i]->id = id;
        array[i]->arrival = arrival;
        array[i]->runtime = runtime;
        array[i]->priority = priority;
        
    }




    fclose(fp);
    


    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    
    int algo = argv[1];
    /*To DO: Read the parameters*/
    
    // 3. Initiate and create the scheduler and clock processes.
    int pid = fork();
    if (pid==0){
        execl("/clk.out",NULL);
    }
    
    // 4. Use this function after creating the clock process to initialize clock.
    
    
    
    initClk();
    
    
    
    // To get time use this function. 
    int x = 0; //getClk();
    
    
    
    printf("Current Time is %d\n", x);
    
    
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    
    
    
    
    
    destroyClk(true);

}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption

    raise(SIGKILL);
}
