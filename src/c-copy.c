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
    // should support arguments: source path, dest path, thread count
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    if (argc < 3)
    {
        printf("Error: Not enough arguments, correct usage: c-copy sourcePath destPath threadCount\n");
        return 1;
    }

    // todo: read paths, check if exist + dirs
    char srcPath[PATH_MAX_LEN];
    char destPath[PATH_MAX_LEN];

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

    ExploreDir(&srcPath, &destPath, &jq);


    pthread_t workers[NUM_THREADS_MAX];

    for (int i = 0; i < threadCount; i++) {
        pthread_create(&workers[i], NULL, WorkerRoutine, NULL);
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(&workers[i], NULL);
    }


    // clean up
    pthread_mutex_destroy(&claim_mutex);

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
        // Optional: Update some shared progress/failure counter based on status and job file size
    }
    return NULL;
}