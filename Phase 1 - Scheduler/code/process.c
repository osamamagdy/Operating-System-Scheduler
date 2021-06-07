#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
bool is_sleep = false;

void sleep_sig(int signum)
{
    is_sleep = true;
}

void awake_sig(int signum)
{
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
    
    //printf("Process started with time = %d\n", remainingtime);
    while (remainingtime > 0)
    {
        int x = getClk();

        if (x > prev)
        {
            prev = x;

            if (!sleep)
                remainingtime--;
        }
        
    }

    destroyClk(false);

    return 0;
}
