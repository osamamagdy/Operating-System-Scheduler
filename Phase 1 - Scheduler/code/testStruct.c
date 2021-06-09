#include<stdio.h>

struct A{
    int a;
};
struct B{
    struct A obj;
    int b;
};
void print(struct A*item)
{
    printf("%d",item->a);
    printf("%d",((struct B*)item)->b);
}
int main(int argc,char**argv)
{
    struct B val;
    val.b=5;
    val.obj.a=6;
    print(&val.obj);
    return 0;
}
