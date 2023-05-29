#ifndef READER_H
#define READER_H
// Path to file
#define PROC_PATH "/proc/stat"
// Definitions fot attempts to open file
#define FIRST_ATTEMPT 0
#define NEXT_ATTEMPT 1
#define MAX_ATTEMPTS 5
// Definitions for reader thread
#define READER_SLEEP_TIME 1
// Function for read from /proc/stat
int read_data(void);
// Function for send readed data to buffer
void send_reader_to_buffer(void);
// Function for set reader internat buffer - for unit test
void reader_set_buffer(char *to_buffer);
// Function for free allocated memory in reader
void free_reader_buffer(void);
// Function for reader thread to work
void *reader_task(void *arg);
// Function for stop reader thread
void stop_reader(void);

#endif
