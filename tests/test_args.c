#include "../include/c-copy-headers.h"
#include "../include/test-headers.h"

int TestThreadCount()
{
    if (GetThreadCount(3, (const char *[]){"./c-copy", "src", "dest", NULL}) != NUM_THREADS_DEFAULT)
    {
        printf("FAIL - default thread count is not implicit\n");
        return -1;
    }
    if (GetThreadCount(4, (const char *[]){"./c-copy", "src", "dest", ":)", NULL}) != NUM_THREADS_DEFAULT)
    {
        printf("FAIL - invalid thread count did not result in default\n");
        return -2;
    }
    if (GetThreadCount(4, (const char *[]){"./c-copy", "src", "dest", "-10", NULL}) != NUM_THREADS_DEFAULT)
    {
        printf("FAIL - negative thread count did not result in default\n");
        return -3;
    }
    if (GetThreadCount(4, (const char *[]){"./c-copy", "src", "dest", "1", NULL}) != 1)
    {
        printf("FAIL - valid thread count was not respected\n");
        return -4;
    }
    if (GetThreadCount(4, (const char *[]){"./c-copy", "src", "dest", STR(NUM_THREADS_MAX + 1), NULL}) != NUM_THREADS_MAX)
    {
        printf("FAIL - excessive thread count did not result in maximum\n");
        return -5;
    }
    return 0;
}

int TestVerifyArgs()
{
    if (GetThreadCount(2, (const char *[]){"1", "2", NULL}) != -1)
    {
        printf("FAIL - did not recognize insufficient argument count\n");
        return -1;
    }
    if (GetThreadCount(5, (const char *[]){"1", "2", "3", "4", "5", NULL}) != -1)
    {
        printf("FAIL - did not recognize excessive argument count\n");
        return -1;
    }
    return 0;
}


int TestArgs()
{
    if (TestVerifyArgs() <= 0 || TestThreadCount() <= 0)
    {
        return -1;
    }
    return 0;
}