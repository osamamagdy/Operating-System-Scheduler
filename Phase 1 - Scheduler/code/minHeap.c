#include "list.c"


struct Heap
{
    process **arr;
    int count;
    int capacity;
};
typedef struct Heap Heap;

Heap *CreateHeap(int capacity);
void insert(Heap *h, struct process *key);
void heapify_bottom_top(Heap *h, int index);
void heapify_top_bottom(Heap *h, int parent_node);
process * PopMin(Heap *h);
int Empty(Heap *h);
void print(Heap *h);
/*
int main()
{
    Heap *heap = CreateHeap(30); //Min Heap
    process X [10] ;
    for(int i = 0; i < 10; i++)
        X[i].id = X[i].arrival = X[i].runtime = 0;
    X[0].priority = 3;
    X[1].priority = 7;
    X[2].priority = 8;
    X[3].priority = 4;
    X[4].priority = 6;
    X[5].priority = 2;
    X[6].priority = 2;
    X[7].priority = 6;
    X[8].priority = 10;
    X[9].priority = 9;
    
    for(int i = 0; i < 10; i++)
    {
        insert(heap, &X[i]);
        print(heap);
    }
    for(int i=9;i>=0;i--){
        printf(" Pop Minima : %d\n", PopMin(heap)->priority);
        print(heap);
    }
    
    return 0;
}*/

struct Heap *CreateHeap(int capacity)
{
    struct Heap *h = (struct Heap *)malloc(sizeof(struct Heap)); //one is number of heap

    h->count = 0;
    h->capacity = capacity;
    h->arr = (struct process **)malloc(capacity * sizeof(struct process *)); //size in bytes

    return h;
}

void insert(struct Heap * h, struct process* key)
{

    if (h->count < h->capacity)
    {
        h->arr[h->count] = key;
        printf("insert process with id %d and PID %d\n",key->id,key->pid);
        heapify_bottom_top(h, h->count);
        h->count++;
    }
    for (int i = 0; i < h->count; i++)
    {
        printf(" id : %d", h->arr[i]->id);
    }
    printf("\n");
    

}

void heapify_bottom_top(struct Heap *h, int index)
{
    printf("Here\n");
    struct process* temp;
    int parent_node = (index - 1) / 2;

    if (h->arr[parent_node]->priority > h->arr[index]->priority)
    {
        //swap and recursive call
        temp = h->arr[parent_node];
        h->arr[parent_node] = h->arr[index];
        h->arr[index] = temp;
        heapify_bottom_top(h, parent_node);
    }
}

void heapify_top_bottom(struct Heap *h, int parent_node)
{
    int left = parent_node * 2 + 1;
    int right = parent_node * 2 + 2;
    int min;
    struct process* temp;

    if (left >= h->count || left < 0)
        left = -1;
    if (right >= h->count || right < 0)
        right = -1;

    if (left != -1 && h->arr[left]->priority < h->arr[parent_node]->priority)
        min = left;
    else
        min = parent_node;
    if (right != -1 && h->arr[right]->priority < h->arr[min]->priority)
        min = right;

    if (min != parent_node)
    {
        temp = h->arr[min];
        h->arr[min] = h->arr[parent_node];
        h->arr[parent_node] = temp;

        // recursive  call
        heapify_top_bottom(h, min);
    }
}

int Empty(struct Heap *h)
{
    return h->count == 0;
}

process *PopMin(struct Heap *h)
{
    struct process *pop;
    if (Empty(h))
    {
        return NULL;
    }
    // replace first node by last and delete last
    pop = h->arr[0];
    h->arr[0] = h->arr[h->count - 1];
    h->count--;
    heapify_top_bottom(h, 0);
    return pop;
}

void print(struct Heap *h){
    int i;
    printf("____________Print Heap_____________\n");
    for(i=0;i< h->count;i++){
        printf("-> %d ",h->arr[i]->priority);
    }
    printf("->__/\\__\n");
}


