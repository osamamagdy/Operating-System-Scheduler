#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
bool is_sleep = false;

int main(int agrc, char *argv[])
{
    //raise(SIGTSTP);
    //printf("====Started====");
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
            prev = x;
            remainingtime--;
            //printf("\n*****remaining time : %d, current time=%d\n", remainingtime, getClk());
            //fflush(stdout);
        }
    }
    //fflush(stdout);

    //printf("\n ******process %d time to exit: %d\n", getpid(), getClk());
    //fflush(stdout);

    destroyClk(false);

    return 0;
}
