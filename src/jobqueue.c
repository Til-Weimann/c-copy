#include <stdio.h>
#include <stdbool.h>
#include "../include/c-copy-headers.h"
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t claim_mutex;

void InitQueue(JobQueue *jq_ptr)
{
    jq_ptr->start = -1;
    jq_ptr->end = 0;
    pthread_mutex_init(&claim_mutex, NULL);
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
        free(job_ptr);
        return false;
    }
    jq_ptr->jobs[jq_ptr->end] = job_ptr;
    jq_ptr->end++;
    return true;
}

CopyJob *ClaimJob(JobQueue *jq_ptr)
{
    CopyJob *job_ptr = NULL;
    pthread_mutex_lock(&claim_mutex);
    if (!IsEmpty(jq_ptr)) 
    {
        job_ptr = jq_ptr->jobs[++jq_ptr->start];
    }
    pthread_mutex_unlock(&claim_mutex);
    return job_ptr;
}
