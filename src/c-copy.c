#include <stdio.h>
#include <c-copy-headers.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>


int main(int argc, char *argv[])
{
    // todo: stdin pipeline integration

    if (argc < 3)
    {
        printf("Usage: %s <source_path> <dest_path> [<threadCount>]\n", argv[0]);
        return 1;
    }

    y
    char *srcPath[PATH_MAX_LEN];
    char *destPath[PATH_MAX_LEN];

	//done: read paths from arg[1] and arg[2] and check if the correlating dir exists
	srcPath = argv[1];
	destPath = argv[2];
	DIR *srcdir;
	DIR *destdir;
	srcdir = opendir(srcPath);
	if(!srcdir)
	{
		return -2;		//-2 indicates invalid source path
	}
	destdir = opendir(destPath);
	if(!destdir)
	{
		closedir(srcPath);
		return -3;		//-3 indicates invalid destination path
	}
	else
	{
		closedir(srcPath);
		closedir(destPath);
	}
	
	

    int threadCount = NUM_THREADS_DEFAULT;
    if (argc > 3)
    {
        int threadArg = atoi(argv[3]);
        if (threadArg > NUM_THREADS_MAX)
        {
            printf("Warning: %d exceeds the thread limit of %d, proceeding with %d.\n", threadArg, NUM_THREADS_MAX);
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

    ExploreDir(&srcdir, &destdir, &jq);

    pthread_t workers[NUM_THREADS_MAX];

    for (int i = 0; i < threadCount; i++) {
        pthread_create(&workers[i], NULL, WorkerRoutine, &jq);
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(&workers[i], NULL);
    }


    // todo: more clean up
    pthread_mutex_destroy(&claim_mutex);
    free(&jq);


    // todo: stdout pipeline integration

}

void ExploreDir(char *srcPath, char *destPath, JobQueue *jq)
{
    DIR *dir;
    struct dirent *entry;
    struct stat st;

    char srcFullPath[PATH_MAX_LEN];
    char destFullPath[PATH_MAX_LEN];

    dir = opendir(srcPath);
    if(!dir){return;}		//could return -1 to indicate error

    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){continue;} //skip for obvious reasons

        snprintf(srcFullPath, PATH_MAX_LEN, "%s/%s", srcPath, entry->d_name);	//path construction
        snprintf(destFullPath, PATH_MAX_LEN, "%s/%s", destPath, entry->d_name);	//assisted by ai



        if(stat(srcFullPath, &st) == -1){continue;}		//if file doesnt exist (anymore?), skip it.


        if(S_ISREG(st.st_mode)) 
		{
            CreateJob(srcFullPath, destFullPath, st.st_size, jq);
        }
        else if(S_ISDIR(st.st_mode)) 
		{
            mkdir(destFullPath, 0755);					//create dir in destination
            ExploreDir(srcFullPath, destFullPath, jq);	//copy dir's contents into destination/dir
        }
    }

    closedir(dir);
}

bool CreateJob(char *jobSrc[], char *jobDest[], int size, JobQueue *jq_ptr)
{
    CopyJob *job_ptr = (CopyJob*) malloc(sizeof(CopyJob));
    if (job_ptr == NULL) {
        printf("Job memory allocation failed.\n");
        return false;
    }
    strcpy(job_ptr->srcPath, *jobSrc);
    strcpy(job_ptr->destPath, *jobDest);
    job_ptr->fileSize = size;

    return Enqueue(jq_ptr, job_ptr);
}


void* WorkerRoutine(void* arg) {
    JobQueue *jq_ptr = arg; 
    JobQueue jq = *jq_ptr;

    CopyJob *job_ptr;
    while ((job_ptr = ClaimJob(jq_ptr)) != NULL)
    {
        int status = CopyFile(job_ptr);
        free(job_ptr);
        // Optional todo: Update some shared progress/failure counter based on status and job file size
    }
    return NULL;
}
