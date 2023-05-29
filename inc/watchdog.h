#ifndef WATCHDOG_H
#define WATCHDOG_H
// Time bofre watchdog check every thread working
#define TIME_TO_CHECK 2
// Time for finish work in all threads before canceled
#define TIME_TO_SAVE 1
// Number of threads to watch by watchdog
#define THREADS_TO_WATCH 4
// ID's of threads
#define READER_ID 0
#define ANALYZER_ID 1
#define PRINTER_ID 2
#define LOGGER_ID 3
// Definitions to specify is thread working
#define CONFIRMED 1
#define NON_CONFIRMED 0
// Functions for  watchdog initialization - set threads to watch
void watchdog_init(pthread_t threads_from_main[], int threads_no);
// Function used by threads to confirm work
void confirm_work(int thread_ID);
// Function for save exit program
void stop_threads(void);
// Function for watchdog thread
void *watchdog_task(void *arg);
// Function for finish warchdog thread
void stop_watchdog(void);

#endif
