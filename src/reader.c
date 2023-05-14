//Includes from standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Includes from my headers
#include "../headers/reader.h"
#include "../headers/global.h"
#include "../headers/buffer.h"

char* reader_internal_buffer = NULL;

//Function for reading data from file 
int read_data()
{
    printf("Readed lines (for verify) : \n");
    FILE* file_info;
   
    //Calloc because with malloc I got some not cleared memory
    reader_internal_buffer = (char*)calloc(LINE_MAX_LENGHT, sizeof(char));

    //In future version I will add proper info to Logger insted of printf
    if (reader_internal_buffer == NULL)
    {
        printf("%s","Error during memory allocation to reader_internal_buffer in read_data");
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
        //Reading data from file to reader_internal_buffer
        while(fgets(line,LINE_MAX_LENGHT,file_info) != NULL)
        {
            //For cpu consumption all I need is lines with cpu 
            if(strncmp(line,CPU_ID,3) != 0)
            {
                break;
            }
            printf("%s \n",line);
            strncat(reader_internal_buffer,line,strlen(line));
        
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
    char* data = (char*)calloc (strlen(reader_internal_buffer),sizeof (char));
    strcpy(data,reader_internal_buffer);
    save_reader_data(data, strlen(data)-1);
    free(reader_internal_buffer);
    reader_internal_buffer = NULL;
    
}