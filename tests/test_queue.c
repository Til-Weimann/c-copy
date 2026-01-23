#include "../include/c-copy-headers.h"
#include "../include/test-headers.h"
#include <stdio.h>
#include <stdlib.h>

int QueueTest1()
{
    JobQueue jq;
    JobQueue *jq_ptr = &jq;
    InitQueue(jq_ptr);

    CopyJob *job_ptr = malloc(sizeof(CopyJob));
    if (job_ptr == NULL)
    {
        fprintf(stderr, "FAIL - job creation failed\n");
        return -1;
    }

    job_ptr->fileSize = 123;

    if (!Enqueue(jq_ptr, job_ptr))
    {
        fprintf(stderr, "FAIL - enqueue failed\n");
        return -2;
    }

    CopyJob* j = ClaimJob(jq_ptr);
    if (j != job_ptr)
    {
        fprintf(stderr, "FAIL - claimed job mismatch\n");
        free(job_ptr);
        return -3;
    }

    free(job_ptr);
    pthread_mutex_destroy(&jq_ptr->mutex);
    return 0;
}


int QueueTest2()
{
    JobQueue jq;
    JobQueue *jq_ptr = &jq;
    InitQueue(jq_ptr);

    CopyJob *job = ClaimJob(jq_ptr);
    if (job != NULL)
    {
        fprintf(stderr, "FAIL - empty queue returned job\n");
        return -1;
    }

    pthread_mutex_destroy(&jq_ptr->mutex);

    return 0;
}

int TestQueue()
{   
    if (QueueTest1() <= 0 || QueueTest2() <= 0)
    {
        return -1;
    }
    return 0;
}
