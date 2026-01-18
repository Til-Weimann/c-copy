#include <stdio.h>
#include "../include/c-copy-headers.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>


int main(int argc, const char *argv[])
{
    // todo: stdin pipeline integration

    time_t start = time(NULL);

    if (argc < 3 || argc > 4)
    {
        printf("Invalid count of arguments, expected usage: %s <source_dir> <dest_dir> [<threadCount>]\n", argv[0]);
        return -1;
	}

	DIR *srcdir;
	DIR *destdir;
	srcdir = opendir(argv[1]);
	if(!srcdir)
	{
        printf("Invalid source directory: %s\n", argv[1]);
		return -2;		//-2 indicates invalid source path
	}
	destdir = opendir(argv[2]);
	if(!destdir)
	{
        mkdir(argv[2], 0755);
		destdir = opendir(argv[2]);
		if(!destdir)
		{
			printf("Invalid destination or failed to create directory: %s\n", argv[2]);
			closedir(srcdir);
			return -3;		//-3 indicates invalid destination path
		}
	}
    closedir(srcdir);
	closedir(destdir);

	
	

    int threadCount = NUM_THREADS_DEFAULT;
    if (argc == 4)
    {
        int threadArg = atoi(argv[3]);
        if (threadArg > NUM_THREADS_MAX)
        {
            printf("Warning: %d exceeds the thread limit of %d, proceeding with %d.\n", threadArg, NUM_THREADS_MAX, NUM_THREADS_MAX);
            threadCount = NUM_THREADS_MAX;
        }
        else if (threadArg <= 0)
        {
            printf("Warning: Thread number %d is invalid, proceeding with %d.\n", threadArg, NUM_THREADS_DEFAULT);
        }
        else
        {
            threadCount = threadArg;
        }
    }

    JobQueue jq;
    InitQueue(&jq);
    pthread_mutex_init(&progress_mutex, NULL);

    ExploreDir(argv[1], argv[2], &jq);

    pthread_t workers[NUM_THREADS_MAX];

    for (int i = 0; i < threadCount; i++) 
	{
        pthread_create(&workers[i], NULL, WorkerRoutine, &jq);
    }

    for (int i = 0; i < threadCount; i++) 
	{
        pthread_join(workers[i], NULL);
    }

    // Leave the progress counter line
    printf("\n");
    fflush(stdout);


    // todo: more clean up
    pthread_mutex_destroy(&claim_mutex);
    pthread_mutex_destroy(&progress_mutex);


    // maybe add:  files copied: n
    printf("Time elapsed: %.1f seconds\n", (double)(time(NULL)-start));
    if (dirs_failed > 0 || files_failed > 0)
    {
        printf("Failed to copy %d directories and %d files\n", dirs_failed, files_failed);
    }
	return 1;
}

bool CreateJob(const char *jobSrc, const char *jobDest, int size, JobQueue *jq_ptr)
{
    CopyJob *job_ptr = (CopyJob*) malloc(sizeof(CopyJob));
    if (job_ptr == NULL)
	{
        return false;
    }
    strcpy(job_ptr->srcPath, jobSrc);
    strcpy(job_ptr->destPath, jobDest);
    job_ptr->fileSize = size;

    return Enqueue(jq_ptr, job_ptr);
}


void* WorkerRoutine(void* arg)
{
    JobQueue *jq_ptr = arg; 

    CopyJob *job_ptr;
    while ((job_ptr = ClaimJob(jq_ptr)) != NULL)
    {
        int status = CopyFile(job_ptr);
        OnJobFinished(status, job_ptr->fileSize);
        free(job_ptr);
    }
    return NULL;
}
