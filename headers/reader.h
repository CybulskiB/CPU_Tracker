#ifndef READER_H
#define READER_H

#define PROC_PATH "/proc/stat"
#define MAX_ATTEMPTS 5

int read_data();
void save_reader_data();
void send_reader_to_buffer();
void reader_set_buffer(char* to_buffer);
void free_reader_buffer();

void* reader_task();

#endif