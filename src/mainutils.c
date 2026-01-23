#include <stdio.h>
#include "../include/c-copy-headers.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


int VerifyArguments(int argc, const char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        #ifndef TESTING
        fprintf(stderr, "Invalid argument count\nExpected usage: %s <source_dir> <dest_dir> [<thread_count>]\n", argv[0]);
        #endif
        return -1;
	}

	DIR *src_dir;
	DIR *dest_dir;
	src_dir = opendir(argv[1]);
	if (!src_dir)
	{
        #ifndef TESTING
        fprintf(stderr, "Invalid source directory: %s\n", argv[1]);
        #endif
		return -2;
	}
	dest_dir = opendir(argv[2]);
	if (!dest_dir)
	{
        mkdir(argv[2], 0755);
		dest_dir = opendir(argv[2]);
		if (!dest_dir)
		{
            #ifndef TESTING
			fprintf(stderr, "Invalid destination or failed to create directory: %s\n", argv[2]);
			#endif
            closedir(src_dir);
			return -3;
		}
	}
    closedir(src_dir);
	closedir(dest_dir);

    return 0;
}


int GetThreadCount(int argc, const char *argv[])
{
    int thread_count = NUM_THREADS_DEFAULT;
    if (argc == 4)
    {
        int thread_arg = atoi(argv[3]);
        if (thread_arg > NUM_THREADS_MAX)
        {
            #ifndef TESTING
            printf("Warning: %d exceeds the thread limit of %d, proceeding with %d.\n", thread_arg, NUM_THREADS_MAX, NUM_THREADS_MAX);
            #endif
            thread_count = NUM_THREADS_MAX;
        }
        else if (thread_arg <= 0)
        {
            #ifndef TESTING
            printf("Warning: Thread number %s is invalid, proceeding with %d.\n", argv[3], NUM_THREADS_DEFAULT);
            #endif
        }
        else
        {
            thread_count = thread_arg;
        }
    }
    return thread_count;
}

bool CreateJob(const char *job_src, const char *job_dest, int size, JobQueue *jq_ptr)
{
    CopyJob *job_ptr = (CopyJob*) malloc(sizeof(CopyJob));
    if (job_ptr == NULL)
	{
        return false;
	}
    strcpy(job_ptr->src_path, job_src);
    strcpy(job_ptr->dest_path, job_dest);
    job_ptr->file_size = size;

    return Enqueue(jq_ptr, job_ptr);
}


void* WorkerRoutine(void* arg)
{
    JobQueue *jq_ptr = arg; 

    CopyJob *job_ptr;
    while ((job_ptr = ClaimJob(jq_ptr)) != NULL)
    {
        int status = CopyFile(job_ptr);
        OnJobFinished(status, job_ptr->file_size);
        free(job_ptr);
    }
    return NULL;
}