//Includes from standard library
#include <stdio.h>
#include <pthread.h>
//Includes from my headers
#include "../headers/analyzer.h"
#include "../headers/global.h"
#include "../headers/reader.h"
#include "../headers/buffer.h"
#include "../headers/printer.h"
#include "../headers/watchdog.h"
#include "../headers/logger.h"

int main()
{
    init_buffer();
    pthread_t reader, analyzer, printer, logger;

    if(pthread_create(&reader,NULL,reader_task,NULL) != 0)
    {
        perror("Failed to create reader thread");
    }
    if(pthread_create(&analyzer,NULL,analyzer_task,NULL) != 0)
    {
        perror("Failed to create analyzer thread");
    }
    if(pthread_create(&printer,NULL,printer_task,NULL) != 0)
    {
        perror("Failed to create printer thread");
    }
    if(pthread_create(&logger,NULL,logger_task,NULL) != 0)
    {
        perror("Faile to create logger thread");
    }

    pthread_t thread_to_watch[THREADS_TO_WATCH] = {reader,analyzer,printer,logger};
    watchdog_init(thread_to_watch,THREADS_TO_WATCH);
    pthread_t watchdog;

    if(pthread_create(&watchdog,NULL,watchdog_task,NULL) != 0)
    {
        perror("Failed to create watchdog thread");
    }

    if(pthread_join(reader,NULL) != 0)
    {
        perror("Failed to join reader thread");
    }
    if(pthread_join(analyzer,NULL) != 0)
    {
        perror("Failed to join analyzer thread");
    }
    if(pthread_join(printer,NULL) != 0)
    {
        perror("Failed to join printer thread");
    }
     if(pthread_join(logger,NULL) != 0)
    {
        perror("Faile to join logger thread");
    }
    if(pthread_join(watchdog,NULL) != 0)
    {
        perror("Failed to join watchdog thread");
    }
    

    destroy_buffer();

    return 0;
}