#include "../include/c-copy-headers.h"
#include "../include/test-headers.h"


int main(void)
{

    int i = MAX_JOBS;
    printf("%i", i);
    GetThreadCount(4, (const char *[]){"./c-copy", "src", "dest", "1", NULL});

    // From here, call the test methods of all test_subject.c files
    
    if (TestArgs() <= 0 && TestFiles() <= 0 && TestQueue() <= 0)
    {
        printf("PASS\n");
        return 0;
    }
    return -1;
}