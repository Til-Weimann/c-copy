#include "../include/c-copy-headers.h"
#include "../include/test-headers.h"

int TestThreadCount()
{
    int fails = 0;
    if (GetThreadCount(3, (const char *[]){"./c-copy", "src", "dest", NULL}) != NUM_THREADS_DEFAULT)
    {
        fprintf(stderr, "FAIL - default thread count is not implicit\n");
        fails++;
    }
    if (GetThreadCount(4, (const char *[]){"./c-copy", "src", "dest", ":)", NULL}) != NUM_THREADS_DEFAULT)
    {
        fprintf(stderr, "FAIL - invalid thread count did not result in default\n");
        fails++;
    }
    if (GetThreadCount(4, (const char *[]){"./c-copy", "src", "dest", "-10", NULL}) != NUM_THREADS_DEFAULT)
    {
        fprintf(stderr, "FAIL - negative thread count did not result in default\n");
        fails++;
    }
    if (GetThreadCount(4, (const char *[]){"./c-copy", "src", "dest", "1", NULL}) != 1)
    {
        fprintf(stderr, "FAIL - valid thread count was not respected\n");
        fails++;
    }
    if (GetThreadCount(4, (const char *[]){"./c-copy", "src", "dest", STR(NUM_THREADS_MAX + 1), NULL}) != NUM_THREADS_MAX)
    {
        fprintf(stderr, "FAIL - excessive thread count did not result in maximum\n");
        fails++;
    }
    return fails;
}

int TestVerifyArgs()
{
    int fails = 0;
    if (VerifyArguments(2, (const char *[]){"1", "2", NULL}) != -1)
    {
        fprintf(stderr, "FAIL - did not recognize insufficient argument count\n");
        fails++;
    }
    if (VerifyArguments(5, (const char *[]){"1", "2", "3", "4", "5", NULL}) != -1)
    {
        fprintf(stderr, "FAIL - did not recognize excessive argument count\n");
        fails++;
    }
    return fails;
}


int TestArgs()
{
    if (TestVerifyArgs() > 0 || TestThreadCount() > 0)
    {
        return -1;
    }
    return 0;
}