#include <stdio.h>
#include <c-copy-headers.h>
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
    // todo: read paths, check if exist

    int threads = 4;
    if (argc > 3)
    {
        threads = atoi(argv[3]);
        // ensure reasonable number
    }

    return 0;

    // explore src dir


    // create multiple threads
    // pthread_t thread;
    // pthread_create(&thread, NULL, WorkerRoutine, NULL);

    for (int i = 0; i < threads; i++) {
        // create thread
    }



    // maybe also update progress percentage whenever a job is finished
}

int CreateJob()
{
    CopyJob job = {"", "", 0};
    return 0;
}


void* WorkerRoutine(void* arg) {
    printf("Created a new thread");
    return NULL;
 }