#include "../include/c-copy-headers.h"
#include "../include/test-headers.h"

void InitQueue(JobQueue *jq_ptr);

int main(void)
{
    
    if (TestArgs() <= 0 && TestFiles() <= 0 && TestQueue() <= 0)
    {
        printf("PASS\n");
        return 0;
    }
    return -1;
}