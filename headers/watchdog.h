#ifndef WATCHDOG_H
#define WATCHDOG_H

#define TIME_TO_CHECK 2

#define THREADS_TO_WATCH 3

#define READER_ID 0
#define ANALYZER_ID 1
#define PRINTER_ID 2

#define CONFIRMED 1
#define NON_CONFIRMED 0

void watchdog_init(pthread_t threads_from_main[], int threads_no);
void* watchdog_task();
void confirm_work(int thread_ID);
void stop_threads();

#endif