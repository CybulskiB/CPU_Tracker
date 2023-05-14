//Includes from standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Includes from my headers
#include "../headers/buffer.h"
#include "../headers/global.h"
#include "../headers/printer.h"

static double cpu_to_print[CPU_MAX] ;
static int cpu_counter;


//Function for print 
void print_data()
{
    if(cpu_counter >0){
        for(int i =1; i <= cpu_counter; i++)
        {
            printf("Cpu's core: %d,  Usage: %.2f%% \n", i,cpu_to_print[i]);
        }
        printf("Total number of core's : %d \n", cpu_counter);
    }
    else{
        printf("Error printer got cpu_counter = %d", cpu_counter);
    }
}

//Function for getting data from buffer
void get_analyzer_data_from_buffer()
{
    send_data_to_printer((double **) &cpu_to_print, &cpu_counter);
}
