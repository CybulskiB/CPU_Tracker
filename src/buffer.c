// Includes from standard library
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Includes from my headers
#include "buffer.h"
#include "global.h"

static int cpu_counter;
static size_t logger_data_size;
// Semaphores and mutexes - solution to Producer Consumer problem
static sem_t reader_analyzer_empty;
static sem_t reader_analyzer_filled;
static sem_t analyzer_printer_empty;
static sem_t analyzer_printer_filled;
static sem_t logger_empty;
static sem_t logger_filled;

static pthread_mutex_t reader_analyzer_mutex;
static pthread_mutex_t analyzer_printer_mutex;
static pthread_mutex_t logger_queue_mutex;

static reader_analyzer_queue *ra_queue;
static analyzer_printer_queue *ap_queue;
static logger_queue *log_queue;

// Function for initialization semaphores
void init_buffer(void) {
  // Queue initialization
  ra_queue = (reader_analyzer_queue *)malloc(sizeof(reader_analyzer_queue));
  ap_queue = (analyzer_printer_queue *)malloc(sizeof(analyzer_printer_queue));
  log_queue = (logger_queue *)malloc(sizeof(logger_queue));
  ra_queue->front = INITIAL_POS;
  ra_queue->rear = INITIAL_POS;
  ap_queue->front = INITIAL_POS;
  ap_queue->rear = INITIAL_POS;
  log_queue->front = INITIAL_POS;
  log_queue->rear = INITIAL_POS;

  // Semaphores initalization
  sem_init(&reader_analyzer_empty, SHARED_BY_THREADS, MAX_NO_DATA_FROM_READER);
  sem_init(&analyzer_printer_empty, SHARED_BY_THREADS,
           MAX_NO_DATA_FROM_ANALYZER);
  sem_init(&logger_empty, SHARED_BY_THREADS, MAX_NO_DATA_TO_LOGGER);
  sem_init(&reader_analyzer_filled, SHARED_BY_THREADS, EMPTY);
  sem_init(&analyzer_printer_filled, SHARED_BY_THREADS, EMPTY);
  sem_init(&logger_filled, SHARED_BY_THREADS, EMPTY);

  // Mutexes initialization
  pthread_mutex_init(&reader_analyzer_mutex, NULL);
  pthread_mutex_init(&analyzer_printer_mutex, NULL);
  pthread_mutex_init(&logger_queue_mutex, NULL);
}

// Function for free memory from buffer
void destroy_buffer(void) {
  if (ra_queue->front <= ra_queue->rear && ra_queue->front != INITIAL_POS &&
      ra_queue->rear != INITIAL_POS) {
    for (int i = ra_queue->front; i <= ra_queue->rear; i++) {
      free(ra_queue->data_from_reader[i]);
      ra_queue->data_from_reader[i] = NULL;
    }
  }
  free(ra_queue);
  ra_queue = NULL;
  free(ap_queue);
  ap_queue = NULL;
  if (log_queue->front <= log_queue->rear && log_queue->front != INITIAL_POS &&
      log_queue->rear != INITIAL_POS) {
    for (int i = log_queue->front; i <= log_queue->rear; i++) {
      free(log_queue->data_to_logger[i]);
      log_queue->data_to_logger[i] = NULL;
    }
  }
  free(log_queue);
  log_queue = NULL;
  sem_destroy(&reader_analyzer_empty);
  sem_destroy(&reader_analyzer_filled);
  sem_destroy(&analyzer_printer_empty);
  sem_destroy(&analyzer_printer_filled);
  sem_destroy(&logger_empty);
  sem_destroy(&logger_filled);

  pthread_mutex_destroy(&reader_analyzer_mutex);
  pthread_mutex_destroy(&analyzer_printer_mutex);
  pthread_mutex_destroy(&logger_queue_mutex);
}

void save_reader_data(char *data, size_t data_size) {
  sem_wait(&reader_analyzer_empty);
  pthread_mutex_lock(&reader_analyzer_mutex);
  if (ra_queue->rear >= MAX_NO_DATA_FROM_READER INITIAL_POS &&
      ra_queue->front == FIRST_POS) {
    save_logger_data("Error: attempt add data to full ra_queue");

  } else { // Optimalization for not refactoring queue all the time after
           // send_data_to_analyzer()
    if (ra_queue->rear >= MAX_NO_DATA_FROM_READER INITIAL_POS &&
        ra_queue->front > FIRST_POS) {
      refactor_ra_queue();
    }
    ra_queue->rear++;
    ra_queue->data_from_reader[ra_queue->rear] = (char *)calloc(
        data_size, sizeof(ra_queue->data_from_reader[ra_queue->rear]));
    if (ra_queue->data_from_reader[ra_queue->rear] == NULL) {
      save_logger_data(
          "Error during memory allocation in  save_data_from_reader");
    } else {
      strncpy(ra_queue->data_from_reader[ra_queue->rear], data, data_size);
    }
    if (ra_queue->front == INITIAL_POS) {
      ra_queue->front = FIRST_POS;
    }
  }
  pthread_mutex_unlock(&reader_analyzer_mutex);
  sem_post(&reader_analyzer_filled);
}

void save_analyzer_data(double cpu_percent_from_analyzer[],
                        int cpu_counter_from_analyzer) {
  sem_wait(&analyzer_printer_empty);
  pthread_mutex_lock(&analyzer_printer_mutex);
  if (ap_queue->rear >= MAX_NO_DATA_FROM_ANALYZER INITIAL_POS &&
      ap_queue->front == FIRST_POS) {
    save_logger_data("Error: attempt add data to full ap_queue\n");

  } else { // Optimalization for not refactoring queue all the time after
           // send_data_to_analyzer()
    if (ap_queue->rear >= MAX_NO_DATA_FROM_ANALYZER INITIAL_POS &&
        ap_queue->front > FIRST_POS) {
      refactor_ap_queue();
    }
    ap_queue->rear++;
    cpu_counter = cpu_counter_from_analyzer;
    memcpy(
        ap_queue->data_from_analyzer[ap_queue->rear], cpu_percent_from_analyzer,
        sizeof(double) * (size_t)(cpu_counter_from_analyzer + CPU_FIRST_LINE));
    if (ap_queue->front == INITIAL_POS) {
      ap_queue->front = FIRST_POS;
    }
  }
  pthread_mutex_unlock(&analyzer_printer_mutex);
  sem_post(&analyzer_printer_filled);
}

void save_logger_data(char *data) {
  sem_wait(&logger_empty);
  pthread_mutex_lock(&logger_queue_mutex);
  logger_data_size = strlen(data);
  if (log_queue->rear >= MAX_NO_DATA_TO_LOGGER INITIAL_POS &&
      log_queue->front == FIRST_POS) {
    save_logger_data("Error: attempt add data to full ra_queue");

  } else { // Optimalization for not refactoring queue all the time after
           // send_data_to_analyzer()
    if (log_queue->rear >= MAX_NO_DATA_TO_LOGGER INITIAL_POS &&
        log_queue->front > FIRST_POS) {
      refactor_log_queue();
    }
    log_queue->rear++;
    log_queue->data_to_logger[log_queue->rear] = (char *)calloc(
        logger_data_size, sizeof(log_queue->data_to_logger[log_queue->rear]));
    if (log_queue->data_to_logger[log_queue->rear] == NULL) {
      save_logger_data(
          "Error during memory allocation in  save_data_from_reader");
    } else {
      strncpy(log_queue->data_to_logger[log_queue->rear], data,
              logger_data_size);
    }
    if (log_queue->front == INITIAL_POS) {
      log_queue->front = FIRST_POS;
    }
  }
  pthread_mutex_unlock(&logger_queue_mutex);
  sem_post(&logger_filled);
}

void send_data_to_analyzer(char **reference_to_analyzer_buffer) {
  sem_wait(&reader_analyzer_filled);
  pthread_mutex_lock(&reader_analyzer_mutex);
  if (ra_queue->front > ra_queue->rear || ra_queue->front == INITIAL_POS) {
    save_logger_data("Error: Underflow in ra_queue\n");
  } else {
    *reference_to_analyzer_buffer =
        (char *)calloc(strlen(ra_queue->data_from_reader[ra_queue->front]),
                       sizeof(reference_to_analyzer_buffer));
    strncpy(*reference_to_analyzer_buffer,
            ra_queue->data_from_reader[ra_queue->front],
            strlen(ra_queue->data_from_reader[ra_queue->front]));
    free(ra_queue->data_from_reader[ra_queue->front]);
    ra_queue->data_from_reader[ra_queue->front] = NULL;
    ra_queue->front++;
  }
  pthread_mutex_unlock(&reader_analyzer_mutex);
  sem_post(&reader_analyzer_empty);
}

void send_data_to_printer(double **reference_to_printer_buffer,
                          int *reference_to_printer_cpu_no) {
  sem_wait(&analyzer_printer_filled);
  pthread_mutex_lock(&analyzer_printer_mutex);
  if (ap_queue->front > ap_queue->rear || ap_queue->front == INITIAL_POS) {
    save_logger_data("Error: Underflow in ap_queue\n");
  } else {
    *reference_to_printer_cpu_no = cpu_counter;
    memcpy(reference_to_printer_buffer,
           ap_queue->data_from_analyzer[ap_queue->front],
           sizeof(double) * (size_t)(cpu_counter + CPU_FIRST_LINE));
    ap_queue->front++;
  }
  pthread_mutex_unlock(&analyzer_printer_mutex);
  sem_post(&analyzer_printer_empty);
}
void send_data_to_logger(char **reference_to_logger_buffer) {
  if (log_queue->front != INITIAL_POS && log_queue->front <= log_queue->rear) {
    sem_wait(&logger_filled);
    pthread_mutex_lock(&logger_queue_mutex);
    *reference_to_logger_buffer =
        (char *)calloc(strlen(log_queue->data_to_logger[log_queue->front]),
                       sizeof(reference_to_logger_buffer));
    strncpy(*reference_to_logger_buffer,
            log_queue->data_to_logger[log_queue->front],
            strlen(log_queue->data_to_logger[log_queue->front]));
    free(log_queue->data_to_logger[log_queue->front]);
    log_queue->data_to_logger[log_queue->front] = NULL;
    log_queue->front++;
    pthread_mutex_unlock(&logger_queue_mutex);
    sem_post(&logger_empty);
  }
}

void refactor_ra_queue(void) {
  if (ra_queue->front > ra_queue->rear) {

    ra_queue->front = INITIAL_POS;
    ra_queue->rear = INITIAL_POS;

  } else {
    int pos = FIRST_POS;
    for (int i = FIRST_POS; i <= ra_queue->rear - ra_queue->front; i++) {
      ra_queue->data_from_reader[i] = (char *)calloc(
          strlen(ra_queue->data_from_reader[ra_queue->front + i]),
          sizeof(ra_queue->data_from_reader[i]));
      strcpy(ra_queue->data_from_reader[i],
             ra_queue->data_from_reader[ra_queue->front + i]);
      free(ra_queue->data_from_reader[ra_queue->front + i]);
      ra_queue->data_from_reader[ra_queue->front + i] = NULL;
      pos = i;
    }
    ra_queue->front = FIRST_POS;
    ra_queue->rear = pos;
  }
}
void refactor_ap_queue(void) {
  if (ap_queue->front > ap_queue->rear) {
    ap_queue->front = INITIAL_POS;
    ap_queue->rear = INITIAL_POS;
  } else {
    int pos = FIRST_POS;
    for (int i = FIRST_POS; i <= ap_queue->rear - ap_queue->front; i++) {

      memcpy(ap_queue->data_from_analyzer[i],
             ap_queue->data_from_analyzer[ap_queue->front + i],
             sizeof(double) * (size_t)(cpu_counter + CPU_FIRST_LINE));
      pos = i;
    }
    ap_queue->front = FIRST_POS;
    ap_queue->rear = pos;
  }
}

void refactor_log_queue(void) {
  if (log_queue->front > log_queue->rear) {
    log_queue->front = INITIAL_POS;
    log_queue->rear = INITIAL_POS;
  } else {
    int pos = FIRST_POS;
    for (int i = FIRST_POS; i <= log_queue->rear - log_queue->front; i++) {
      log_queue->data_to_logger[i] = (char *)calloc(
          strlen(log_queue->data_to_logger[log_queue->front + i]),
          sizeof(log_queue->data_to_logger[i]));
      strcpy(log_queue->data_to_logger[i],
             log_queue->data_to_logger[log_queue->front + i]);
      free(log_queue->data_to_logger[log_queue->front + i]);
      log_queue->data_to_logger[log_queue->front + i] = NULL;
      pos = i;
    }
    log_queue->front = FIRST_POS;
    log_queue->rear = pos;
  }
}
