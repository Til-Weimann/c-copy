#include <stdio.h>

typedef struct {
    char srcPath[4096+ 255];
    char destPath[4096+ 255];
    int fileSize;
} CopyJob;