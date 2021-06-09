#include <stdlib.h>
#include <stdio.h>

struct f_Node
{

    int start;
    int size;
    struct f_Node *next;
    struct f_Node *prev;
    
};


struct f_List
{
    struct f_Node *head;
    struct f_Node *tail;
    int size;
};

void f_initList(struct f_List *l)
{
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

void f_insert_after(struct f_List * l, int address , int size)
{
    struct f_Node *n = (struct f_Node *) malloc(sizeof(struct f_Node));

    struct f_Node * curr = l->head;

    if (curr == NULL)
    {
        n->start = 0;
        n->size = size;
        n->next =NULL;
        l->head = n;
        return;
    }
    while ( curr != NULL && ((curr->start + curr->size)< address)  )
    {
        curr = curr->next;
    }
    n->start = address;
    n->size = size;
    n->next = curr->next;
    curr->next = n;
    return;


}

void f_RemoveAt(struct f_List *l, short processStart)
{
    l->size--;
    struct f_Node *n = l->head;
    while (n)   
    {
        if(n->start == processStart)
        {
            if(n == l->head)
            {
                l->head = n->next;
                l->head->prev = NULL;
            }
            else if(n == l->tail)
            {
                l->tail = n->prev;
                l->tail->next = NULL;
            }
            else
            {
                (n->prev)->next = n->next;
                (n->next)->prev = n->prev;
            }
            return;
        } 
        n = n->next;
    }
}

void f_PrintList(struct f_List *l)
{
    struct f_Node *n = l->head;

    while (n)
    {
        printf("{start : %d, size : %d \n", n->start, n->size);
        n = n->next;   
    }
    
}
