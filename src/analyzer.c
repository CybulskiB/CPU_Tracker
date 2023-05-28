//Includes from standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

//Includes from my headers
#include "../headers/analyzer.h"
#include "../headers/global.h"
#include "../headers/buffer.h"
#include "../headers/watchdog.h"

static char* analyzer_internal_buffer = NULL;
static double cpu_percent[CPU_MAX];
static int cpu_counter;
static size_t prev_idle[CPU_MAX], prev_iowait[CPU_MAX];
static size_t prev_user[CPU_MAX],prev_nice[CPU_MAX],prev_system[CPU_MAX];
static size_t prev_irq[CPU_MAX],prev_softirq[CPU_MAX],prev_steal[CPU_MAX];
static size_t curr_idle,curr_iowait = CURR_INIT;
static size_t curr_user,curr_nice,curr_system,curr_irq,curr_softirq,curr_steal = CURR_INIT;
static size_t PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total;
static size_t totald, idled;
int analyzer_control = WORKING;
static int analyze_no = FIRST;


void* analyzer_task()
{

    while(analyzer_control) 
    {
        confirm_work(ANALYZER_ID);
        get_reader_data_from_buffer();
        analyze_data();
        send_analyzer_to_buffer();
        free_analyzer_buffer();

    }
    return NULL;
}
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
        char* line_copy = (char *) malloc(sizeof(line_copy) * strlen(proc_stat_line));
        strcpy(line_copy,proc_stat_line);
        cpu_use_info = strtok(line_copy,INFO_SEPARATOR);
        if(cpu_counter != 0)
        {
            cpu_use_info = strtok(NULL,INFO_SEPARATOR);
        }
        set_curr(cpu_use_info);
        
        if(analyze_no)
        {
            PrevIdle = PREV_INIT;
            PrevNonIdle = PREV_INIT;
            PrevTotal = PREV_INIT;
            analyze_no = NEXT;

        }
        else
        {
            PrevIdle = prev_idle[cpu_counter] + prev_iowait[cpu_counter];
            PrevNonIdle = prev_user[cpu_counter] + prev_nice[cpu_counter] + prev_system[cpu_counter] \
            + prev_irq[cpu_counter] + prev_softirq[cpu_counter] + prev_steal[cpu_counter];
            PrevTotal = PrevIdle + PrevNonIdle;
        }
        Idle = curr_idle + curr_iowait;
        NonIdle = curr_user + curr_nice + curr_system + curr_irq + curr_softirq + curr_steal;
        Total = Idle + NonIdle;
        totald = Total - PrevTotal;
        idled = Idle - PrevIdle;
        cpu_percent[cpu_counter] = 100.0 *  (double)(totald -idled)/totald;
        set_prev(cpu_counter);

        proc_stat_line = strtok_r(proc_stat_line_pos,CPU_ID,&proc_stat_line_pos);

        if(cpu_counter > CPU_MAX)
        {
            save_logger_data("Unexpected high number of cpu in analyze_data()");
        }

        free(line_copy);
        
    }

}
void set_curr(char* cpu_line)
{
    int infos_counter = 0;
    while(cpu_line != NULL)
    {
        switch (infos_counter){
            case 0:
                curr_user = strtoul(cpu_line,NULL,NUM_BASE);
                break;
            case 1:
                curr_nice = strtoul(cpu_line,NULL,NUM_BASE);
                break;
            case 2:
                curr_system = strtoul(cpu_line,NULL,NUM_BASE);
                break;
            case 3:
                curr_idle = strtoul(cpu_line,NULL,NUM_BASE);
                break;
            case 4:
                curr_iowait = strtoul(cpu_line,NULL,NUM_BASE);
                break;
            case 5:
                curr_irq = strtoul(cpu_line,NULL,NUM_BASE);
                break;
            case 6: 
                curr_softirq = strtoul(cpu_line,NULL,NUM_BASE);
                break;
            case 7:
                curr_steal = strtoul(cpu_line,NULL,NUM_BASE);
                break;
            default:
                break;
            }
        infos_counter ++;
        cpu_line = strtok(NULL,INFO_SEPARATOR);
    }
}

void set_prev(int position)
{
    if(position > CPU_MAX)
    {
        save_logger_data("Error: Analyzer got to high position in set prev");
        stop_analyzer();
    }
    else
    {
        prev_user[position] = curr_user;
        prev_nice[position] = curr_nice;
        prev_system[position] = curr_system;
        prev_idle[position] = curr_idle;
        prev_iowait[position] = curr_iowait;
        prev_irq[position] = curr_irq;
        prev_softirq[position] = curr_softirq;
        prev_steal[position] = curr_steal;
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
void free_analyzer_buffer()
{
    free(analyzer_internal_buffer);
    analyzer_internal_buffer= NULL;

}
void stop_analyzer()
{
    analyzer_control = END_THREAD;
}


