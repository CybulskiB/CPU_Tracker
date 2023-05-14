#ifndef READER_H
#define READER_H

#define PROC_PATH "/proc/stat"

int read_data();
void save_reader_data();
void send_reader_to_buffer();

#endif