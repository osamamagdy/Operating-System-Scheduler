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
        heapify_bottom_top(h, h->count);
        h->count++;
    }
    
    printf("\n");
    

}

void heapify_bottom_top(struct Heap *h, int index)
{

    struct process* temp;
    int parent_node = (index - 1) / 2;

    if (h->arr[parent_node]->priority > h->arr[index]->priority || (h->arr[parent_node]->priority == h->arr[index]->priority && h->arr[parent_node]->arrival > h->arr[index]->arrival))
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

    if (left != -1 && (h->arr[left]->priority < h->arr[parent_node]->priority || (h->arr[left]->priority == h->arr[parent_node]->priority && h->arr[left]->arrival < h->arr[parent_node]->arrival) ))
        min = left;
    else
        min = parent_node;
    if (right != -1 && (h->arr[right]->priority < h->arr[min]->priority || (h->arr[right]->priority == h->arr[min]->priority && h->arr[right]->arrival < h->arr[min]->arrival)))
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


