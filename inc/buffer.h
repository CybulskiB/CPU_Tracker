#ifndef BUFFER_H
#define BUFFER_H

// Max number of cpus
#define CPU_MAX 33
// Max sizes of the queues
#define MAX_NO_DATA_FROM_ANALYZER 100
#define MAX_NO_DATA_FROM_READER 100
#define MAX_NO_DATA_TO_LOGGER 100
// Initial positions in queue
#define INITIAL_POS -1
#define FIRST_POS 0
// Definitions for semaphore initialization
#define EMPTY 0
#define SHARED_BY_THREADS 0
// For memory allocation with first line in /proc/stat
#define CPU_FIRST_LINE 1
// Function for saving data from analyzer
void save_analyzer_data(double cpu_percent_from_analyzer[],
                        int cpu_counter_from_analyzer);
// Function for saving data from reader
void save_reader_data(char *data, size_t data_size);
// Function for saving data for logger
void save_logger_data(char *data);
// Function for sending data to analyzer
void send_data_to_analyzer(char **reference_to_analyzer_buffer);
// Function for sending data to printer
void send_data_to_printer(double **reference_to_printer_buffer,
                          int *reference_to_printer_cpu_no);
void send_data_to_logger(char **reference_to_logger_buffer);
// Initialization of queues, semaphores and mutexes
void init_buffer(void);
// Free memory for queues, semaphores and mutexes
void destroy_buffer(void);
// Refactor queue reader->analyzer
void refactor_ra_queue(void);
// Refactor queue analyzer -> printer
void refactor_ap_queue(void);
// Refactor queue threads -> logger
void refactor_log_queue(void);

// Definition of queues
//   for case where there are different buffers size I madde different
//   definitions
typedef struct {
  char *data_from_reader[MAX_NO_DATA_FROM_READER];
  int front;
  int rear;
} reader_analyzer_queue;

typedef struct {
  double data_from_analyzer[MAX_NO_DATA_FROM_ANALYZER][CPU_MAX];
  int front;
  int rear;
} analyzer_printer_queue;

typedef struct {
  char *data_to_logger[MAX_NO_DATA_TO_LOGGER];
  int front;
  int rear;
} logger_queue;

#endif
