#include "headers.h"
#pragma once

inline void RoundRobin (const int msgq_id, int sem1, int sem2,const int timeQunatum)
{
    struct List ready_queue;
    initList(&ready_queue);
    ready_queue.tail->next=ready_queue.head;
}