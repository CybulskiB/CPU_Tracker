/*
Belowed code isn't write for tests with assertion, but for 
checking memory leaks. Result program should be run with valgrind
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../headers/global.h"
#include "../headers/reader.h"
#include "../headers/analyzer.h"
#include "../headers/buffer.h"
#include "../headers/printer.h"
#include "../headers/logger.h"
#include "../headers/watchdog.h"
#include "watchdog_memory_check.h"

static pthread_t reader, analyzer, printer, logger, watchdog;

static void* (*threads_functions[ALL_THREADS])(void *) = {&reader_task,&analyzer_task,&printer_task,&logger_task,&watchdog_task};

void* not_respond()
{
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

    sleep(WAIT_BEFORE_STOP);
    stop_reader();
    //stop_analyzer();
    //stop_printer();
    //stop_logger();

    for(size_t i = 0; i < ALL_THREADS; i++)
    {
        if(pthread_join(threads[i],NULL) != 0)
        {
            perror("Failed to join all threads");
        }
    }
    destroy_buffer();
    return NULL;
}


int main()
{
    pthread_t reader_check;
    printf("Starting not respond simulation \n");
    if(pthread_create(&reader_check,NULL,not_respond,NULL) != 0)
    {
        perror("Failed to create reader check thread");
    }
    if(pthread_join(reader_check,NULL) != 0 )
    {
        perror("Failed to join reader check thread");
    }
    printf("Not simulation finished \n");

    return 0;
}