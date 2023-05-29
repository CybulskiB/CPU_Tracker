// Includes from standard library
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// Includes from my headers
// In visual studio code modyfication include paths
#include "analyzer.h"
#include "buffer.h"
#include "global.h"
#include "logger.h"
#include "printer.h"
#include "reader.h"
#include "watchdog.h"

static pthread_t threads_to_watch[THREADS_TO_WATCH];
static pthread_t threads[ALL_THREADS];
// Functions used by threads
static void *(*threads_functions[ALL_THREADS])(void *) = {
    &reader_task, &analyzer_task, &printer_task, &logger_task, &watchdog_task};
// Functions called when program receive SIGTERM or SIGINT
static void terminate(int signum) {
  if (signum == SIGTERM || signum == SIGINT) {
    printf("Received a signal %d \n", signum);
    stop_threads();
  }
}

int main(void) {
  signal(SIGTERM, *terminate);
  signal(SIGINT, *terminate);
  printf("%d\n", getpid());
  init_buffer();

  // Starting threads
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

  // Waiting for finish threads
  for (size_t i = NO_THREADS; i < ALL_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != SUCCESS) {
      perror("Failed to join all threads");
    }
  }
  // Free memory from buffer
  destroy_buffer();

  return SUCCESS;
}
