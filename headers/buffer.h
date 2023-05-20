#ifndef BUFFER_H
#define BUFFER_H

//Max number of cpus
#define CPU_MAX 33

#define MAX_NO_DATA_FROM_ANALYZER 100 //I've made some test for that values in {1,2,3,4,5,6,7,8,9,10} 
#define MAX_NO_DATA_FROM_READER 100    // with func_test for check refactoring
//Functionality functions
void send_data_to_printer(double** reference_to_printer_buffer, int* reference_to_printer_cpu_no);
void save_analyzer_data(double cpu_percent_from_analyzer[], int cpu_counter_from_analyzer);
void send_data_to_analyzer(char** reference_to_analyzer_buffer);
//Queue and multithreating functions
void init_buffer();
void destroy_buffer();
void refactor_ra_queue();
void refactor_ap_queue();

//Definition of queues 
//  for case where there are different buffers size I madde different definitions 
//  for queue reader-> analyzer and analyzer -> printer
typedef struct{
    char* data_from_reader[MAX_NO_DATA_FROM_READER];
    int front;
    int rear;
} reader_analyzer_queue;

typedef struct{
    double data_from_analyzer[MAX_NO_DATA_FROM_ANALYZER][CPU_MAX];
    int front;
    int rear;
} analyzer_printer_queue;

#endif