#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
//Includes from my headers
#include "../headers/logger.h"
#include "../headers/buffer.h"
#include "../headers/global.h"
#include "../headers/watchdog.h"

static char* logger_internal_buffer = NULL;
static FILE* log_file;
int logger_control = WORKING;

void* logger_task()
{
    while(logger_control)
    {
        confirm_work(LOGGER_ID);
        get_logs();
        if (logger_internal_buffer != NULL)
        {
            log_file = fopen(LOG_FILENAME, "a+");
            if(log_file == NULL)
            {
                printf("Error: Failed to open log file");
            }
            else
            {
                fprintf(log_file,"%s\n",logger_internal_buffer);

            }
            free_logger_buffer();
        }
    }
    return NULL;
}
void get_logs()
{
    send_data_to_logger(&logger_internal_buffer);
}

void free_logger_buffer()
{
    free(logger_internal_buffer);
    logger_internal_buffer = NULL;
}
void stop_logger()
{
    logger_control = END_THREAD;
}