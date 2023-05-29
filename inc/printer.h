#ifndef PRINTER_H

#define PRINTER_H
// Definition for checking printer got any cpu to print
#define NO_CPU 0
// Function for connect printer with buffer
void get_analyzer_data_from_buffer(void);
// Function for print
void print_data(void);

// Function for  printer thread
void *printer_task(void *arg);
// Function for finish printer work
void stop_printer(void);

#endif
