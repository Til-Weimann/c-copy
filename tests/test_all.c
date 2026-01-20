#include "../include/c-copy-headers.h"
#include "../include/test-headers.h"

void InitQueue(JobQueue *jq_ptr);

int main(void)
{
    // From here, call the test methods of all test_subject.c files

    JobQueue jq;
    InitQueue(&jq);
    
    //pthread_mutex_init(&claim_mutex, NULL); // seg fault

    return 0;
    
    if (TestArgs() <= 0 && TestFiles() <= 0 && TestQueue() <= 0)
    {
        printf("PASS\n");
        return 0;
    }
    return -1;
}