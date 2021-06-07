#include "headers.h"
#include "list.c"

struct List ready_queue;

int main(int argc, char *argv[])
{
    initClk();
    initList(&ready_queue);

    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.

    key_t key_id;
    int  msgq_id;

    key_id = ftok("keyfile", 65);               //create unique key
    msgq_id = msgget(key_id, 0666 | IPC_CREAT); //create message queue and return id
    if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }

    FCFS_SC(msgq_id);

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