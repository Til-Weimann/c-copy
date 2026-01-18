#include "../include/c-copy-headers.h"
#include "./test-headers.h"
#include <stdio.h>

int main(void)
{

    // From here, call the test methods of all test_subject.c files
    
    if (false)
    {
        printf("FAIL - reason\n");
        return -1;
    }

    if (false)
    {
        printf("FAIL - reason\n");
        return -2;
    }

    printf("PASS\n");
    return 0;
}