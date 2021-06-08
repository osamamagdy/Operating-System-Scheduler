#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
bool is_sleep = false;

void sleep_sig(int signum)
{
    printf("Process %d sleep\n",getpid());
    is_sleep = true;

}

void awake_sig(int signum)
{
    printf("Process %d Wake \n",getpid());
    is_sleep = false;
}

int main(int agrc, char *argv[])
{
    signal (SIGUSR1, sleep_sig);
    signal (SIGUSR2, awake_sig);
    initClk();

    //TODO The process needs to get the remaining time from somewhere
    
    /**
     * @todo check for ههههههه
    */
    remainingtime = atoi(argv[1]);
    int prev = getClk();
    
    printf("Process %d started with time = %d\n", getpid() ,remainingtime);
    while (remainingtime > 0)
    {
        int x = getClk();

        if (x > prev)
        {
            printf("Process %d in time slot %d\n",getpid(), x);
            prev = x;

            if (!is_sleep)
                remainingtime--;
        }
        
    }

    printf("Process %d terminating\n",getpid());
    return 0;
}
