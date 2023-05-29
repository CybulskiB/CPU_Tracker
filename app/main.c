//Includes from standard library
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
//Includes from my headers
#include "../headers/analyzer.h"
#include "../headers/global.h"
#include "../headers/reader.h"
#include "../headers/buffer.h"
#include "../headers/printer.h"
#include "../headers/watchdog.h"
#include "../headers/logger.h"
static pthread_t reader, analyzer, printer, logger, watchdog;
static void* (*threads_functions[ALL_THREADS])(void *) = {&reader_task,&analyzer_task,&printer_task,&logger_task,&watchdog_task};

void terminate(int signum)
{
    if(signum == SIGTERM || signum == SIGINT){
        printf("Received a signal %d \n", signum);
        stop_threads();
        
    }
}


int main()
{
    signal(SIGTERM, *terminate);
    signal(SIGINT, *terminate);
    printf("%d\n", getpid());
    init_buffer();

    pthread_t threads_to_watch[THREADS_TO_WATCH];
    pthread_t threads[ALL_THREADS] = {reader,analyzer,printer,logger,watchdog};
    for(size_t i = 0; i < ALL_THREADS; i++)
    {
        if( i == THREADS_TO_WATCH )
        {
            memcpy(threads_to_watch,threads,sizeof(pthread_t) * THREADS_TO_WATCH);
            watchdog_init(threads_to_watch,THREADS_TO_WATCH);
        }
        if(pthread_create(&threads[i],NULL,threads_functions[i], NULL) != 0)
        {
            perror("Failed to create threads");
        }
    }

   
    for(size_t i = 0; i < ALL_THREADS; i++)
    {
        if(pthread_join(threads[i],NULL) != 0)
        {
            perror("Failed to join all threads");
        }
    }

    destroy_buffer();

    return 0;
}

