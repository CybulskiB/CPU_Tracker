//Includes from standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
//Includes from my headers
#include "../headers/buffer.h"
#include "../headers/global.h"
#include "../headers/printer.h"
#include "../headers/watchdog.h"
#include "../headers/logger.h"

static double cpu_to_print[CPU_MAX] ;
static int cpu_counter;
static char* error_message;
int printer_control = WORKING;

void* printer_task()
{
   
    while(printer_control) 
    {
        confirm_work(PRINTER_ID);
        get_analyzer_data_from_buffer();
        print_data();
    }
    return NULL;
}


//Function for print 
void print_data()
{
    
    //Print data
    if(cpu_counter >0){
        for(int i =1; i <= cpu_counter; i++)
        {
            printf("Cpu's core: %d,  Usage: %.2f%% \n", i,cpu_to_print[i]);
        }
        printf("Total number of core's : %d \n", cpu_counter);
        printf( "\n \n \n");
    }
    else{
        error_message = (char*)malloc(sizeof(char) * ERROR_MESSAGE_SIZE);
        sprintf(error_message,"Error printer got cpu_counter = %d ", cpu_counter);
        save_logger_data(error_message);
        free(error_message);
        
    }
}

//Function for getting data from buffer
void get_analyzer_data_from_buffer()
{
    send_data_to_printer((double **) &cpu_to_print, &cpu_counter);
}

void stop_printer()
{
    printer_control = END_THREAD;
}
