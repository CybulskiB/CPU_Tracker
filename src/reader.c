//Includes from standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Includes from my headers
#include "../headers/reader.h"
#include "../headers/global.h"
#include "../headers/buffer.h"

static char* internal_buffer = NULL;

//Reference to function for reader thread
void* reader_task()
{
    while(1)
    {
        read_data();
        send_reader_to_buffer();
    }
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
        printf("%s","Error during memory allocation to internal_buffer in read_data");
        return 1;
    }

    file_info = fopen(PROC_PATH,"r");
    if(file_info == NULL)
    {
        printf("%s",strcat("Couldn't open ",PROC_PATH));
        return 1;
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
        //    printf("%s \n",line);
            strncat(internal_buffer,line,strlen(line));
        
        }
        free(line);
        line = NULL;
        fclose(file_info);
        return 0;        
    }
}

//Sending data to analyzer
void send_reader_to_buffer()
{
    int internal_buffer_lo = strlen(internal_buffer);
    char* data = (char*)calloc (internal_buffer_lo,sizeof(data));
    strncpy(data,internal_buffer,internal_buffer_lo);
    save_reader_data(data, strlen(data));
    free(internal_buffer);
    internal_buffer = NULL;
    free(data);
    data = NULL;
    
}
//Function for testing 
void reader_set_buffer(char* to_buffer)
{
    internal_buffer = (char *) calloc(strlen(to_buffer),sizeof(internal_buffer));
    strcpy(internal_buffer,to_buffer);

}