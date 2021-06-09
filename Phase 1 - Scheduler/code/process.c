#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
bool is_sleep = false;

void sleep_sig();
void awake_sig();

int main(int agrc, char *argv[])
{
    signal(SIGTSTP, sleep_sig);
    signal(SIGCONT, awake_sig);
    initClk();

    //TODO The process needs to get the remaining time from somewhere

    /**
     * @todo check for ههههههه
    */
    remainingtime = atoi(argv[1]);
    int prev = getClk();

    printf("Process %d :  started with time = %d\n", getpid(), remainingtime);
    while (remainingtime > 0)
    {
        int x = getClk();

        if (x > prev)
        {
            prev = x;

            if (!is_sleep)
            {
                remainingtime--;
                printf("Process %d time remaining %d\n", getpid(), remainingtime);
            }
        }
    }

    printf("Process %d terminating\n", getpid());
    return 0;
}

void sleep_sig(int signum)
{
    sleep(1);
    printf("Process %d sleep\n", getpid());
    is_sleep = true;
}

void awake_sig(int signum)
{
    sleep(1);
    printf("Process %d Wake \n", getpid());
    is_sleep = false;
}
