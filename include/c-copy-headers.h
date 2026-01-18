#include <stdbool.h>
#include <pthread.h>

#define MAX_JOBS 5000
#define PATH_MAX_LEN 4096

#define NUM_THREADS_MAX 16
#define NUM_THREADS_DEFAULT 4

typedef struct {
    char srcPath[4096 + 255]; // Make change to define value
    char destPath[4096 + 255];
    unsigned long int fileSize; // supports 4TiB
} CopyJob;

typedef struct {
    CopyJob *jobs[MAX_JOBS];
    int start;
    int end;
} JobQueue;


// make sure to update these when changing method signatures!

// fileutils
int CopyFile(CopyJob *job_ptr);

// jobqueue
extern pthread_mutex_t claim_mutex;
void InitQueue(JobQueue *jq_ptr);
bool IsEmpty(JobQueue *jq_ptr);
bool IsFull(JobQueue *jq_ptr);
bool Enqueue(JobQueue *jq_ptr, CopyJob *job_ptr);
CopyJob *ClaimJob(JobQueue *jq_ptr);

// main
void* WorkerRoutine(void* arg);
void ExploreDir(const char *srcPath, const char *destPath, JobQueue *jq);
bool CreateJob(const char *jobSrc, const char *jobDest, int size, JobQueue *jq_ptr);

// progress
extern pthread_mutex_t progress_mutex;
extern unsigned long int bytes_total;
extern unsigned long int bytes_done;
extern unsigned long int bytes_failed;
extern unsigned long int files_copies_failed;
extern unsigned long int dir_copies_failed;
void OnJobFinished(int status, unsigned long int size);