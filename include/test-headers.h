#include <stdio.h>

// macro recommended by chatgpt
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// In here, add method signatures of all test methods called from test_all.c
int TestQueue();
int TestFiles();
int TestArgs();

// No idea why these are needed here, it should already be included via c-copy-headers.h
int GetThreadCount(int argc, const char *argv[]);
int VerifyArguments(int argc, const char *argv[]);