#include "../include/c-copy-headers.h"
#include "../include/test-headers.h"
#include <stdio.h>
#include <stdlib.h>

int QueueTest1()
{
    JobQueue jq;
    
    InitQueue(&jq); // seg fault

    return 0;

    CopyJob *job = malloc(sizeof(CopyJob));
    if (job == NULL)
    {
        fprintf(stderr, "FAIL - job creation failed\n");
        return -1;
    }

    job->fileSize = 123;

    if (!Enqueue(&jq, job))
    {
        fprintf(stderr, "FAIL - enqueue failed\n");
        return -2;
    }

    CopyJob *claimed = ClaimJob(&jq);
    if (claimed != job)
    {
        fprintf(stderr, "FAIL - claimed job mismatch\n");
        return -3;
    }

    free(claimed);
    pthread_mutex_destroy(&claim_mutex);
    return 0;
}


int QueueTest2()
{
    JobQueue jq;
    InitQueue(&jq);

    CopyJob *job = ClaimJob(&jq);
    if (job != NULL)
    {
        fprintf(stderr, "FAIL - empty queue returned job\n");
        return -1;
    }

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
