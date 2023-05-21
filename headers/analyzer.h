#ifndef ANALYZER_H
#define ANALYZER_H
//Separator for single line in proc/stat
#define INFO_SEPARATOR " "
//Integer value to increment by counting cpu
#define START_COUNT_CPU -1
//To store usage in processor in percent 
#define TO_PERCENT 100

void send_analyzer_to_buffer();
void get_reader_data_from_buffer();
void analyze_data();
void delete_data_from_reader();
void free_analyzer_buffer();

void* analyzer_task();

#endif