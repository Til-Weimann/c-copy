#include <stdio.h>
#include "../include/c-copy-headers.h"
#include <stdlib.h>

pthread_mutex_t progress_mutex;

unsigned long int bytes_total = 0;
unsigned long int bytes_done = 0;
unsigned long int bytes_failed = 0;

unsigned long int files_failed = 0;
unsigned long int dirs_failed = 0;


void PrintProgressUpdate()
{
    if (bytes_total == 0)
    {
        return;
    }

    float progress_rate = 100 * (bytes_done + bytes_failed) / bytes_total;
    float failure_rate = 100 * bytes_failed / bytes_total;

    printf("\rProgress: %0.1f%% (%0.1f%% failed)", progress_rate, failure_rate);
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
        files_failed++;
    }

    pthread_mutex_unlock(&progress_mutex);

    PrintProgressUpdate();
}