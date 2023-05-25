#ifndef LOGGER_H
#define LOGGER_H

#define LOG_FILENAME "Log_file.txt"

void* logger_task();
void get_logs();
void free_logger_internal_buffer();

#define ERROR_MESSAGE_SIZE 100

#endif 