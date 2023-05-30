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
#include "reader.h"
#include "watchdog.h"

static char *internal_buffer = NULL;
static char *error_message;
static unsigned int reader_control = WORKING;
static unsigned int attempts_to_open = FIRST_ATTEMPT;
void *reader_task(void *arg) {
  if (arg != NULL) {
    save_logger_data("Warning: Reader got unexpected argument");
  }
  while (reader_control) {

    confirm_work(READER_ID);
    if (read_data() == SUCCESS) {
      attempts_to_open = FIRST_ATTEMPT;
      send_reader_to_buffer();
      free_reader_buffer();
    } else {
      free_reader_buffer();
      if (attempts_to_open == MAX_ATTEMPTS) {
        save_logger_data("Unable to open file with proc info - reader exit");
        break;
      } else {
        attempts_to_open += NEXT_ATTEMPT;
      }
    }
    sleep(READER_SLEEP_TIME);
  }
  return NULL;
}

int read_data(void) {
  FILE *file_info;
  internal_buffer = (char *)calloc(CPU_MAX * LINE_MAX_LENGHT, sizeof(char));

  if (internal_buffer == NULL) {
    save_logger_data(
        "Error during memory allocation to internal_buffer in read_data");
    return FAIL;
  }

  file_info = fopen(PROC_PATH, "r");
  if (file_info == NULL) {
    error_message = (char *)malloc(sizeof(char) * ERROR_MESSAGE_SIZE);
    strcpy(error_message, "Couldn't open ");
    strcat(error_message, PROC_PATH);
    save_logger_data(error_message);
    free(error_message);
    return FAIL;
  }

  else {
    char *line = (char *)malloc(sizeof(char) * LINE_MAX_LENGHT);
    // Reading data from file to internal_buffer
    while (fgets(line, LINE_MAX_LENGHT, file_info) != NULL) {
      // For cpu consumption all I need is lines with cpu
      if (strncmp(line, CPU_ID, LEN_CPU_ID) != SUCCESS) {
        break;
      }

      strncat(internal_buffer, line, strlen(line));
    }
    free(line);
    line = NULL;
  }
  fclose(file_info);
  return SUCCESS;
}

void send_reader_to_buffer(void) {

  save_reader_data(internal_buffer, strlen(internal_buffer));
}
void free_reader_buffer(void) {
  free(internal_buffer);
  internal_buffer = NULL;
}
void stop_reader(void) { reader_control = END_THREAD; }

void reader_set_buffer(char *to_buffer) {
  internal_buffer = (char *)calloc(strlen(to_buffer), sizeof(internal_buffer));
  strcpy(internal_buffer, to_buffer);
}
