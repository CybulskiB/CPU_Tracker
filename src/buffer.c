//Includes from standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Includes from my headers
#include "../headers/buffer.h"
#include "../headers/global.h"

static char* data_from_reader = NULL;
static double cpu_percent[CPU_MAX];
static int cpu_counter;


//Function for saving data from reader to buffer
void save_reader_data(char* data, size_t data_size)
{
    data_from_reader = (char *)calloc(data_size, sizeof(char));
    if(data_from_reader == NULL)
    {
        printf("%s","Error during memory allocation in  save_data_from_reader");
    }
    else
    {
        strncpy(data_from_reader,data,data_size);
    }
}
//Function for saving data from analyzer to buffer 
void save_analyzer_data(double cpu_percent_from_analyzer[], int cpu_counter_from_analyzer)
{

    cpu_counter = cpu_counter_from_analyzer;
    memcpy(cpu_percent,cpu_percent_from_analyzer,sizeof(double) * (cpu_counter_from_analyzer +1));

}
//Sending data to analyzer by passing by reference
void send_data_to_analyzer(char** reference_to_analyzer_buffer)
{
    *reference_to_analyzer_buffer = (char *) calloc(strlen(data_from_reader),sizeof(char));
    strncpy(*reference_to_analyzer_buffer,data_from_reader,strlen(data_from_reader));

}
//Sending data to printer by passyng by reference
void send_data_to_printer(double** reference_to_printer_buffer, int* reference_to_printer_cpu_no)
{
    *reference_to_printer_cpu_no = cpu_counter;
    memcpy(reference_to_printer_buffer,cpu_percent,sizeof(double) * (cpu_counter +1));
}

//Function for deleting data from reader in buffer
void delete_data_from_reader()
{
    free(data_from_reader);
    data_from_reader = NULL;
}