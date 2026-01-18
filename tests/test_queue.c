#include "../include/c-copy-headers.h"
#include "../include/test-headers.h"
#include <stdio.h>

int QueueTest1()
{
    // Implement some test here
    return 0;
}

int QueueTest2()
{
    // Implement some test here
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