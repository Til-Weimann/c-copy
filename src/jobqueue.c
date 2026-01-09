#include <stdio.h>
#include <stdbool.h>
#include <c-copy-headers.h>

void InitQueue(JobQueue *jq)
{
    jq->start = -1;
    jq->end = 0;
}

bool IsEmpty(JobQueue *jq)
{
    return (jq->start + 1 == jq->end);
}

bool IsFull(JobQueue *jq)
{
    return (jq->end == MAX_JOBS);
}

bool Enqueue(JobQueue *jq, CopyJob *job)
{
    if (IsFull(jq)) {
        printf("Job Queue is full\n");
        free(job);
        return false;
    }
    jq->jobs[jq->end] = job;
    jq->end++;
    return true;
}

CopyJob *Claim(JobQueue *jq)
{
    if (isEmpty(jq)) {
        printf("Queue is empty\n");
        return NULL;
    }
    return jq->jobs[++jq->start];
    // don't forget to later free struct memory when job is done!
}