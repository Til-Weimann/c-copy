#include <stdio.h>
#include <stdbool.h>
#include "../include/c-copy-headers.h"
#include <pthread.h>
#include <stdlib.h>

void InitQueue(JobQueue *jq_ptr)
{
    jq_ptr->start = -1;
    jq_ptr->end = 0;
    pthread_mutex_init(&jq_ptr->mutex, NULL);
}

bool IsEmpty(JobQueue *jq_ptr)
{
    return (jq_ptr->start + 1 == jq_ptr->end);
}

bool IsFull(JobQueue *jq_ptr)
{
    return (jq_ptr->end == MAX_JOBS);
}

bool Enqueue(JobQueue *jq_ptr, CopyJob *job_ptr)
{
    if (IsFull(jq_ptr))
    {
        return false;
    }
    jq_ptr->jobs[jq_ptr->end] = job_ptr;
    jq_ptr->end++;
    return true;
}

CopyJob *ClaimJob(JobQueue *jq_ptr)
{
    CopyJob *job_ptr = NULL;
    pthread_mutex_lock(&jq_ptr->mutex);
    if (!IsEmpty(jq_ptr)) 
    {
        job_ptr = jq_ptr->jobs[++jq_ptr->start];
    }
    pthread_mutex_unlock(&jq_ptr->mutex);
    return job_ptr;
}
