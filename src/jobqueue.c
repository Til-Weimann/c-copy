#include <stdio.h>
#include <stdbool.h>
#include <c-copy-headers.h>
#include <pthread.h>

pthread_mutex_t claim_mutex;

void InitQueue(JobQueue *jq)
{
    jq->start = -1;
    jq->end = 0;
    pthread_mutex_init(&claim_mutex, NULL);
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

CopyJob *ClaimJob(JobQueue *jq)
{
    pthread_mutex_lock(&claim_mutex);
    if (isEmpty(jq)) {
        printf("Queue is empty\n");
        return NULL;
    }
    CopyJob *job = jq->jobs[++jq->start];
    pthread_mutex_unlock(&claim_mutex);
    return job;
    // don't forget to later free struct memory when job is done!
}