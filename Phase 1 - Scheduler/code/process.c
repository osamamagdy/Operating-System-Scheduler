#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
bool is_sleep = false;

void sleep_sig(int signum)
{
    printf("Sleep %d\n",getpid());
    is_sleep = true;

}

void awake_sig(int signum)
{
    printf("Wake %d\n",getpid());
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
            printf("Process now in time slot %d\n", x);
            prev = x;

            if (!is_sleep)
                remainingtime--;
        }
        
    }

    printf("A7A\n");
    destroyClk(false);

    return 0;
}
