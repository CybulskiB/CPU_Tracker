#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Includes from my headers
#include "analyzer.h"
#include "buffer.h"
#include "global.h"
#include "logger.h"
#include "printer.h"
#include "reader.h"
#include "watchdog.h"

static int watchdog_working = CONFIRMED;
static int threads_alive[THREADS_TO_WATCH];
static pthread_t threads[THREADS_TO_WATCH];
static char *error_message;

void watchdog_init(pthread_t threads_from_main[], int threads_no) {
  if (threads_no != THREADS_TO_WATCH) {
    save_logger_data("Error: Watchdog got unexpected number of threads");
  } else {
    memcpy(threads, threads_from_main,
           sizeof(pthread_t) * (unsigned long)threads_no);
  }
}
void *watchdog_task(void *arg) {
  if (arg != NULL) {
    save_logger_data("Warning: Watchdog thread got unexpected argument");
  }
  while (watchdog_working) {
    sleep(TIME_TO_CHECK);
    for (size_t i = NO_THREADS; i < THREADS_TO_WATCH; i++) {
      if (threads_alive[i] == NON_CONFIRMED && watchdog_working == CONFIRMED) {
        error_message = (char *)malloc(sizeof(char *) * ERROR_MESSAGE_SIZE);
        sprintf(error_message, "Error: thread %ld didn't confirm work", i);
        save_logger_data(error_message);
        free(error_message);
        stop_threads();
      }
      threads_alive[i] = NON_CONFIRMED;
    }
  }
  return NULL;
}

void confirm_work(int thread_ID) {
  if (thread_ID >= THREADS_TO_WATCH) {
    save_logger_data("Error: Watchdog got unexpected thread ID");
  } else {
    threads_alive[thread_ID] = CONFIRMED;
  }
}

void stop_threads(void) {
  stop_reader();
  stop_analyzer();
  stop_watchdog();
  stop_logger();
  stop_printer();
  sleep(TIME_TO_SAVE);

  for (size_t i = NO_THREADS; i < THREADS_TO_WATCH; i++) {
    pthread_cancel(threads[i]);
  }

  free_reader_buffer();
  free_analyzer_buffer();
  free_logger_buffer();
}
void stop_watchdog(void) { watchdog_working = NON_CONFIRMED; }
