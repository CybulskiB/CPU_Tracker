//Includes from standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
//Includes from my headers
#include "../headers/reader.h"
#include "../headers/global.h"
#include "../headers/buffer.h"
#include "../headers/watchdog.h"
#include "../headers/logger.h"

static char* internal_buffer = NULL;
static char* error_message;
int reader_control = WORKING;

//Reference to function for reader thread
void* reader_task()
{
    size_t attempts_to_open = 0;
    while(reader_control) 
    {
        
        confirm_work(READER_ID);
        if(read_data() ==  SUCCESS)
        {
            send_reader_to_buffer();
            free_reader_buffer();
        }
        else
        {
            free_reader_buffer();
            if(attempts_to_open == MAX_ATTEMPTS)
            {
                save_logger_data("Unable to open file with proc info - reader exit");
                break;
            }
            else
            {
                attempts_to_open +=1;
            }
        }
        sleep(1);
    }
    return NULL;
}

//Function for reading data from file 
int read_data()
{
  //  printf("Readed lines (for verify) : \n");
    FILE* file_info;
   
    //Calloc because with malloc I got some not cleared memory
    internal_buffer = (char*)calloc(LINE_MAX_LENGHT, sizeof(char));

    //In future version I will add proper info to Logger insted of printf
    if (internal_buffer == NULL)
    {
        save_logger_data("Error during memory allocation to internal_buffer in read_data");
        return FAIL;
    }

    file_info = fopen(PROC_PATH,"r");
    if(file_info == NULL)
    {
        error_message = (char*) malloc(sizeof(char) * ERROR_MESSAGE_SIZE);
        strcpy(error_message,"Couldn't open ");
        strcat(error_message,PROC_PATH);
        save_logger_data(error_message);
        free(error_message);
        return FAIL;
    }

    else
    {
        char* line = (char *) malloc(sizeof(char) *LINE_MAX_LENGHT);
        //Reading data from file to internal_buffer
        while(fgets(line,LINE_MAX_LENGHT,file_info) != NULL)
        {
            //For cpu consumption all I need is lines with cpu 
            if(strncmp(line,CPU_ID,3) != 0)
            {
                break;
            }
        
            strncat(internal_buffer,line,strlen(line));
        
        }
        free(line);
        line = NULL;
        fclose(file_info);
        return SUCCESS;        
    }
}

//Sending data to analyzer
void send_reader_to_buffer()
{
  
    save_reader_data(internal_buffer, strlen(internal_buffer));
 
}
void free_reader_buffer()
{
    free(internal_buffer);
    internal_buffer = NULL;
}
//Function for testing 
void reader_set_buffer(char* to_buffer)
{
    internal_buffer = (char *) calloc(strlen(to_buffer),sizeof(internal_buffer));
    strcpy(internal_buffer,to_buffer);

}
void stop_reader()
{
    reader_control = END_THREAD;
}