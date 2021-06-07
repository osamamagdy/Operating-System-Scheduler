#include "headers.h"

int main(int argc, char *argv[])
{
    initClk();

    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.



    sleep(50);
    destroyClk(true);
}


void SJF(){
    Heap PQ();
    int currRemaining = 0; 
    while(!empty(PQ) && !empty(array))
    {
        if(process arrived)
            PQ.push(proccess);
        if(currRemaining) 
            currRemaining--;
        else
        {
            process = PQ.pop();
            currRemaining = process.runtime;
            start(process);
        }
    }
}