#include <stdbool.h>
#include <pthread.h>

#define MAX_JOBS 1000
#define PATH_MAX_LEN 4096

#define NUM_THREADS_MAX 16
#define NUM_THREADS_DEFAULT 4

typedef struct {
    char srcPath[4096 + 255];
    char destPath[4096 + 255];
    int fileSize;
} CopyJob;

typedef struct {
    CopyJob *jobs[MAX_JOBS];
    int start;
    int end;
} JobQueue;


int CopyFile(CopyJob *job_ptr);

bool Enqueue(JobQueue *jq_ptr, CopyJob *job_ptr);
CopyJob *ClaimJob(JobQueue *jq_ptr);
pthread_mutex_t claim_mutex;