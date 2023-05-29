// Includes from standard library
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Includes from my headers
#include "buffer.h"
#include "global.h"
#include "logger.h"
#include "watchdog.h"
// Variables for logger to work
static char *logger_internal_buffer = NULL;
static FILE *log_file;
// Variable for control logger work
static int logger_control = WORKING;

void *logger_task(void *arg) {
  if (arg != NULL) {
    save_logger_data("Warning: Logger got unexpected argument");
  }
  while (logger_control) {
    confirm_work(LOGGER_ID);
    get_logs();
    if (logger_internal_buffer != NULL) {
      log_file = fopen(LOG_FILENAME, "a+");
      if (log_file == NULL) {
        printf("Error: Failed to open log file");
      } else {
        fprintf(log_file, "%s\n", logger_internal_buffer);
      }
      free_logger_buffer();
      fclose(log_file);
    }
  }
  return NULL;
}
void get_logs(void) { send_data_to_logger(&logger_internal_buffer); }

void free_logger_buffer() {
  free(logger_internal_buffer);
  logger_internal_buffer = NULL;
}
void stop_logger(void) { logger_control = END_THREAD; }
