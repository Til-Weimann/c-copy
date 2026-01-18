#include <stdio.h>
#include <stdbool.h>
#include "../include/c-copy-headers.h"
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t progress_mutex;

unsigned long int bytes_total = 0;
unsigned long int bytes_done = 0;
unsigned long int bytes_failed = 0;

unsigned long int files_copies_failed = 0;
unsigned long int dir_copies_failed = 0;


void PrintProgressUpdate()
{
    // Files copied: 20% (failed: 0%)
    // calculate ratios then print update

    if (bytes_total == 0)
    {
        return;
    }

    float progressRate = 100 * (bytes_done + bytes_failed) / bytes_total;
    float failureRate = 100 * bytes_failed / bytes_total;

    printf("\rProgress: %0.1f%% (%0.1f%% failed)", progressRate, failureRate);
    fflush(stdout);

    // This requires that nothing else is printed during the copy phase!
}

void OnJobFinished(int status, unsigned long int size)
{
    pthread_mutex_lock(&progress_mutex);

    if (status == 0)
    {
        bytes_done += size;
    }
    else
    {
        bytes_failed += size;
    }

    pthread_mutex_unlock(&progress_mutex);

    PrintProgressUpdate();
}
