#ifndef LOGGER_H
#define LOGGER_H

#define LOG_FILENAME "Log_file.txt"
#define ERROR_MESSAGE_SIZE 100
// Function for logger thread
void *logger_task(void *arg);
// Function for getting data to logger from buffer
void get_logs(void);
// Function for free memory in logger
void free_logger_buffer(void);
// Function for finish logger work
void stop_logger(void);

#endif
