#include <stdio.h>
#include "../include/c-copy-headers.h"
#include <stdlib.h>
#include <time.h>


int main(int argc, const char *argv[])
{

    clock_t start = clock();

    int args_valid = VerifyArguments(argc, argv);
    if (args_valid < 0)
        return args_valid;

    const int thread_count = GetThreadCount(argc, argv);

    JobQueue jq;
    JobQueue *jq_ptr = &jq;
    InitQueue(jq_ptr);

    pthread_mutex_init(&progress_mutex, NULL);

    ExploreDir(argv[1], argv[2], jq_ptr);
    
    PrintProgressUpdate();

    pthread_t workers[NUM_THREADS_MAX];

    for (int i = 0; i < thread_count; i++) 
	{
        pthread_create(&workers[i], NULL, WorkerRoutine, jq_ptr);
    }

    for (int i = 0; i < thread_count; i++) 
	{
        pthread_join(workers[i], NULL);
    }

    pthread_mutex_destroy(&jq_ptr->mutex);
    pthread_mutex_destroy(&progress_mutex);


    // Leave the progress counter line
    printf("\n");
    fflush(stdout);


    float time_passed = (float)(clock()-start) / CLOCKS_PER_SEC;
    printf("Time elapsed: %0.2f seconds\n", time_passed);
    if (dirs_failed > 0 || files_failed > 0)
    {
        printf("Failed to copy %lu directories and %lu files\n", dirs_failed, files_failed);
    }
	return 1;
}