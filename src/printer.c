// Includes from standard library
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Includes from my headers
#include "buffer.h"
#include "global.h"
#include "logger.h"
#include "printer.h"
#include "watchdog.h"
// Variables to get from vuffer
static double cpu_to_print[CPU_MAX];
static int cpu_counter;
// Variable for printer thread
static int printer_control = WORKING;

void *printer_task(void *arg) {
  if (arg != NULL) {
    save_logger_data("Warning: Printer got unexpected argument");
  }
  while (printer_control) {
    confirm_work(PRINTER_ID);
    get_analyzer_data_from_buffer();
    print_data();
  }
  return NULL;
}

// Function for print
void print_data(void) {

  // Print data
  if (cpu_counter > NO_CPU) {
    printf("Total cpu core's Usage %.2f%% \n", cpu_to_print[0]);
    for (int i = 1; i <= cpu_counter; i++) {
      printf("Cpu's core: %d,  Usage: %.2f%% \n", i, cpu_to_print[i]);
    }
    printf("Total number of core's : %d \n", cpu_counter);
    printf("\n \n \n");
  } else {
    char *error_message = (char *)malloc(sizeof(char) * ERROR_MESSAGE_SIZE);
    sprintf(error_message, "Error printer got cpu_counter = %d ", cpu_counter);
    save_logger_data(error_message);
    free(error_message);
  }
}

// Function for getting data from buffer
void get_analyzer_data_from_buffer(void) {
  send_data_to_printer((double **)&cpu_to_print, &cpu_counter);
}

void stop_printer(void) { printer_control = END_THREAD; }
