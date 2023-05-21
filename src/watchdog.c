#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
//Includes from my headers
#include "../headers/reader.h"
#include "../headers/analyzer.h"
#include "../headers/buffer.h"
#include "../headers/global.h"
#include "../headers/watchdog.h"

static int watchdog_working = CONFIRMED;
static int threads_alive[THREADS_TO_WATCH];
pthread_t threads[THREADS_TO_WATCH];

void watchdog_init(pthread_t threads_from_main[], int threads_no)
{
    if(threads_no != THREADS_TO_WATCH)
    {
        printf("Error: Watchdog got unexpected number of threads");
    }
    else
    {
        memcpy(threads,threads_from_main,sizeof(pthread_t) * threads_no);
    }
}
void* watchdog_task()
{
    while(watchdog_working)
    {
        sleep(TIME_TO_CHECK);
        for(int i =0; i< THREADS_TO_WATCH; i++)
        {
            if(threads_alive[i] == NON_CONFIRMED)
            {
                watchdog_working = NON_CONFIRMED;
                printf("Error: thread %d didn't confirm work \n", i);
                stop_threads();
            }
            threads_alive[i] = NON_CONFIRMED;
        }
    }
    return NULL;
}
void confirm_work(int thread_ID)
{
    if(thread_ID >= THREADS_TO_WATCH)
    {
        printf("Error: Watchdog got unexpected thread ID");
    }
    else
    {
        threads_alive[thread_ID] = CONFIRMED;
    }    
}
void stop_threads()
{
    for(int i = 0; i< THREADS_TO_WATCH; i++)
    {
        pthread_cancel(threads[i]);
    }
    free_reader_buffer();
    free_analyzer_buffer();
}
