#include <stdio.h>
#include <stdbool.h>
#include <copyjob.c>

#define MAX_JOBS 1000

typedef struct {
    CopyJob* items[MAX_JOBS];
    int start;
    int end;
} JobQueue;

void InitQueue(JobQueue* jq)
{
    jq->start = -1;
    jq->end = 0;
}

bool IsEmpty(JobQueue* jq)
{
    return (jq->start + 1 == jq->end);
}

bool IsFull(JobQueue* jq)
{
    return (jq->end == MAX_JOBS);
}

void Enqueue(JobQueue* jq, CopyJob* job)
{
    if (IsFull(jq)) {
        printf("Job Queue is full\n");
        return;
    }
    jq->items[jq->end] = job;
    jq->end++;
}

void Dequeue(JobQueue* jq)
{
    if (isEmpty(jq)) {
        printf("Queue is empty\n");
        return;
    }
    jq->start++;
}