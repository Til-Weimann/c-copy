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

    time_t start = time(NULL);

    int argsValid = VerifyArguments(argc, argv);
    if (argsValid < 0)
        return argsValid;

    int threadCount = GetThreadCount(argc, argv);

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
        printf("Failed to copy %lu directories and %lu files\n", dirs_failed, files_failed);
    }
	return 1;
}