#include <stdlib.h>
#include <stdio.h>

typedef struct process
{
    int id;
    int arrival;
    int runtime;
    int priority;
    int pid;
    int start_time;
    int runtime_log;
    int total_wait;
    int last_run;
    /* data */
}process;

struct Node
{
    struct process data;
    struct Node *next;
};


struct List
{
    struct Node *head;
    struct Node *tail;
    int size;
};

struct msgbuff
{
    long mtype;
    struct process p;
};


void initList(struct List *l)
{
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

void enqueue(struct List *l, struct process *p)
{
    struct Node *n = (struct Node *) malloc(sizeof(struct Node));
    n->data = *p;
    n->next = NULL;

    if (l->size == 0)
    {
        l->head = n;
        l->tail = n;
    }
    else
    {
        l->tail->next = n;
        l->tail = n;
    }
    l->size++;
}

void dequeue(struct List *l)
{
    if (l->size == 0) return ;

    struct Node *dNode = l->head;
    l->head = l->head->next;
    free(dNode);

    l->size--;
}

void PrintList(struct List *l)
{
    struct Node *n = l->head;

    while (n)
    {
        printf("{%d, %d, %d, %d}\n", n->data.id, n->data.arrival, n->data.priority, n->data.runtime);
        n = n->next;   
    }
    
}
