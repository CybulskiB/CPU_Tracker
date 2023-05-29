#ifndef ANALYZER_H
#define ANALYZER_H
// Separator for single line in proc/stat
#define INFO_SEPARATOR " "
// Positions infromation in line from /proc/stat idexed from 0
#define USER_POS 0
#define NICE_POS 1
#define SYSTEM_POS 2
#define IDLE_POS 3
#define IOWAIT_POS 4
#define IRQ_POS 5
#define SOFTIRQ_POS 6
#define STEAL_POS 7
#define GUEST_POS 8
// Definitions for initialization variables with information from /proc/stat
#define PREV_INIT 0
#define CURR_INIT 0
// Integer value to increment by counting cpu
#define START_COUNT_CPU -1
// Definition to store usage in processor in percent
#define TO_PERCENT 100.0
// Definitions for cpu_counter to spcifi it is first line in proc/stat
#define CPU_TOTAL 0
// Definition for specify number system in strtoul function
#define NUM_BASE 10
// Definitions to specify it is first analysis
#define FIRST 1
#define NEXT 0
// Function for send analyzed data to buffer
void send_analyzer_to_buffer(void);
// Function for get reader data from buffer
void get_reader_data_from_buffer(void);
// Function for memory allocation
void delete_data_from_reader(void);
// Function for free memory in analyzer internal data
void free_analyzer_buffer(void);
// Function for analysis
void analyze_data(void);
// Function for setting new prevs cpu's infos
void set_prev(int position);
// Function for settign new current cpu's infos
int set_curr(char *cpu_line);
// Function for analyzer threads
void *analyzer_task(void *arg);
// Function for finish analyzer work
void stop_analyzer(void);

#endif
