#include "../include/c-copy-headers.h"
#include "../include/test-headers.h"

int main(void)
{
    printf("Running tests...\n");
    if (TestArgs() <= 0 && TestQueue() <= 0)
    {
        printf("Testing complete, PASS\n");
        return 0;
    }
    printf("Testing complete, FAIL\n");
    return -1;
}