/*
Belowed code isn't write for tests with assertion, but for
checking memory leaks. Result program should be run with valgrind
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../inc/analyzer.h"
#include "../inc/buffer.h"
#include "../inc/global.h"
#include "../inc/logger.h"
#include "../inc/printer.h"
#include "../inc/reader.h"
#include "../inc/watchdog.h"
#include "watchdog_memory_check.h"

static pthread_t simulation;
static pthread_t threads_to_watch[THREADS_TO_WATCH];
static pthread_t threads[ALL_THREADS];

static void *(*threads_functions[ALL_THREADS])(void *) = {
    &reader_task, &analyzer_task, &printer_task, &logger_task, &watchdog_task};

static void *not_respond() {
  init_buffer();
  for (size_t i = NO_THREADS; i < ALL_THREADS; i++) {
    if (i == THREADS_TO_WATCH) {
      memcpy(threads_to_watch, threads, sizeof(pthread_t) * THREADS_TO_WATCH);
      watchdog_init(threads_to_watch, THREADS_TO_WATCH);
    }
    if (pthread_create(&threads[i], NULL, threads_functions[i], NULL) !=
        SUCCESS) {
      perror("Failed to create threads");
    }
  }

  sleep(WAIT_BEFORE_STOP);
  stop_reader();
  // stop_analyzer();
  // stop_printer();
  // stop_logger();

  for (size_t i = NO_THREADS; i < ALL_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != SUCCESS) {
      perror("Failed to join all threads");
    }
  }
  destroy_buffer();
  return NULL;
}

int main() {
  printf("Starting not respond simulation \n");
  if (pthread_create(&simulation, NULL, not_respond, NULL) != SUCCESS) {
    perror("Failed to create reader check thread");
  }
  if (pthread_join(simulation, NULL) != SUCCESS) {
    perror("Failed to join reader check thread");
  }
  printf("Not simulation finished \n");

  return SUCCESS;
}
