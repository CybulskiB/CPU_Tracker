//Includes from standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Includes from my headers
#include "../headers/analyzer.h"
#include "../headers/global.h"
#include "../headers/buffer.h"

static char* analyzer_internal_buffer = NULL;
static double cpu_percent[CPU_MAX];
static int cpu_counter;
//Function for analyze
void analyze_data()
{

    char* proc_stat_line;
    char* cpu_use_info;
    char* proc_stat_line_pos = analyzer_internal_buffer;

    cpu_counter = START_COUNT_CPU;
    proc_stat_line = strtok_r(analyzer_internal_buffer, CPU_ID, &proc_stat_line_pos);
    while(proc_stat_line != NULL)
    {
        cpu_counter++;
        char* line_copy = (char *) malloc(sizeof(char) * strlen(proc_stat_line));
        strcpy(line_copy,proc_stat_line);
        cpu_use_info = strtok(line_copy,INFO_SEPARATOR);
        if(cpu_counter == 0)
        {
            cpu_percent[cpu_counter] = atof(cpu_use_info);
        }
        else
        {
            cpu_use_info = strtok(NULL,INFO_SEPARATOR);
            cpu_percent[cpu_counter] = TO_PERCENT *  atof(cpu_use_info)/cpu_percent[0];
        }

        proc_stat_line = strtok_r(proc_stat_line_pos,CPU_ID,&proc_stat_line_pos);

        if(cpu_counter > CPU_MAX)
        {
            printf("%s","Unexpected high number of cpu in analyze_data()");
        }
        
    }

}
//Function for getting data to analyze from buffer
void get_reader_data_from_buffer()
{
    send_data_to_analyzer(&analyzer_internal_buffer);
}
void send_analyzer_to_buffer()
{
    save_analyzer_data(cpu_percent,cpu_counter);
}