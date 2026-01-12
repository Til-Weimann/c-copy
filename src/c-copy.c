#include <stdio.h>
#include <c-copy-headers.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
//#include <pthread.h>

int main(int argc, char *argv[])
{
    // should support arguments: source path, dest path, thread count
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    if (argc < 3)
    {
        printf("Error: Not enough arguments supplied");
        return 1;
    }

    // todo: read paths, check if exist + dirs
    char srcPath[4096];
    char destPath[4096];

    int threads = 4;
    if (argc > 3)
    {
        threads = atoi(argv[3]);
        // ensure reasonable number
    }

    return 0;

    JobQueue jq;
    InitQueue(&jq);

    ExploreDir(&srcPath, &destPath, &jq);


    // create multiple threads
    // pthread_t thread;
    // pthread_create(&thread, NULL, WorkerRoutine, NULL);

    for (int i = 0; i < threads; i++) {
        // create thread
    }

}

void ExploreDir(char *srcPath, char *destPath, JobQueue *jq)
{
    DIR *dir;
    struct dirent *entry;
    struct stat st;

    char srcFullPath[PATH_MAX_LEN];
    char destFullPath[PATH_MAX_LEN];

    dir = opendir(srcPath);
    if(!dir){return;}		//could return -1

    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){continue;} //skip for obvious reasons

        snprintf(srcFullPath, PATH_MAX_LEN, "%s/%s", srcPath, entry->d_name);	//path construction
        snprintf(destFullPath, PATH_MAX_LEN, "%s/%s", destPath, entry->d_name);	//assisted by ai



        if(stat(srcFullPath, &st) == -1){continue;}	//if file doesnt exist (anymore?), skip it.


        if(S_ISREG(st.st_mode)) 
	{
            CreateJob(srcFullPath, jobFullPath, jq);
        }
        else if(S_ISDIR(st.st_mode)) 
	{
            mkdir(destFullPath, 0755);			//create dir in destination
            ExploreDir(srcFullPath, destFullPath, q);	//copy dir's contents into destination/dir
        }
    }

    closedir(dir);
}

bool CreateJob(char *jobSrc[], char *jobDest[], int size, JobQueue *jq)
{
    CopyJob *job_ptr = (CopyJob*) malloc(sizeof(CopyJob));
    if (job_ptr == NULL) {
        printf("Job memory allocation failed.\n");
        return false;
    }
    strcpy(job_ptr->srcPath, *jobSrc);
    strcpy(job_ptr->destPath, *jobDest);
    job_ptr->fileSize = size;

    return Enqueue(jq, job_ptr);
}


void* WorkerRoutine(void* arg) { // needs the queue pointer
    printf("Created a new worker thread");
    // Create loop that claims a new job from the queue, calls CopyFile with the job, then frees the job pointer
    // When the job list is empty, quit
    // Optional: After finishing a job, update some progress counter based on job file size
    return NULL;
 }
