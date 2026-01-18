#include <stdio.h>

// macro recommended by chatgpt
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// In here, add method signatures of all test methods called from test_all.c
int TestQueue();
int TestFiles();
int TestArgs();
