typedef struct CopyJob {
    char srcPath[4096 + 255];
    char destPath[4096 + 255];
    int fileSize;
} CopyJob;

int CopyFile(CopyJob *job);