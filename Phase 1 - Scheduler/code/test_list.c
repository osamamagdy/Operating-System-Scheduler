#include "list.c"

int main()
{
    struct List l;
    initList(&l);

    int ans = 0;
    struct process *p;
    while (1)
    {
        printf("Enter 1 for insert new node\n");
        printf("2 for delete first node\n");
        printf("3 to print list\n");
        scanf("%i", &ans);

        switch (ans)
        {
        case 1:
            printf("\nEnter value to inset\n");
            scanf("%i", &ans);
            p = (struct process *)malloc(sizeof(struct process));
            p->id = ans;
            p->arrival = 0;
            p->priority = 0;
            p->runtime = 0;
            
            enqueue(&l, p);
            printf("value added\n");
            break;
        
        case 2:
            dequeue(&l);
            printf("first node deleted\n");
            break;
        case 3:
            PrintList(&l);
            break;
        default:
            break;
        }
    }
    
    
}