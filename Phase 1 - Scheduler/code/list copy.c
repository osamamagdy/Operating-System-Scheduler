    #include <stdlib.h>
    #include <stdio.h>

    typedef struct process
    {
        int id;
        int arrival;
        int runtime;
        int priority;
        int pid;
        int memsize;
        /* data */
    } process;

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

    void initList(struct List *l)
    {
        l->head = NULL;
        l->tail = NULL;
        l->size = 0;
    }

    void enqueue(struct List *l, struct process *p)
    {
        struct Node *n = (struct Node *)malloc(sizeof(struct Node));
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
        if (l->size == 0)
            return;

        struct Node *dNode = l->head;
        l->head = l->head->next;
        free(dNode);

        l->size--;
    }

void RemoveAt(struct List *l, int index)
{
    l->size--;
    int c = 0;
    struct Node *n = l->head;
    struct Node *prev = l->head;
    while (n)   
    {
        if(c == index)
        {
            if(n == l->head)
                l->head = n->next;
            else if(n == l->tail)
                l->tail = prev;
            else
                prev->next = n->next;
            return;
        } 
        prev = n;
        n = n->next;
        c++;
    }
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
