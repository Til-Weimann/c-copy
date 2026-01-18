#include <stdio.h>
#include "../include/c-copy-headers.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>


int VerifyArguments(int argc, const char *argv[])
{
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

    return 0;
}


int GetThreadCount(int argc, const char *argv[])
{
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
    return threadCount;
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