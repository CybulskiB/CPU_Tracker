#ifndef BUFFER_H
#define BUFFER_H

//Max number of cpus
#define CPU_MAX 33

void send_data_to_printer(double** reference_to_printer_buffer, int* reference_to_printer_cpu_no);
void save_analyzer_data(double cpu_percent_from_analyzer[], int cpu_counter_from_analyzer);
void send_data_to_analyzer(char** reference_to_analyzer_buffer);

#endif