#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
bool is_sleep = false;

void sleep_sig(int signum)
{
    is_sleep = true;
    signal(SIGTSTP, sleep_sig);
}

void awake_sig(int signum)
{
    is_sleep = false;
    signal(SIGCONT, awake_sig);
}

int main(int agrc, char *argv[])
{
    //signal(SIGTSTP, sleep_sig);
    //signal(SIGCONT, awake_sig);
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
            printf("\n remaining time : %d, current time=%d\n", remainingtime, getClk());
            prev = x;
            remainingtime--;
        }
    }
    while (prev >= getClk())
        ;
    printf("\n process %d time to exit: %d\n", getpid(), getClk());
    destroyClk(false);

    return 0;
}
