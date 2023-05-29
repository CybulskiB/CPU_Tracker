#ifndef GLOBAL_H
#define GLOBAL_H

// Expected max size of length  from /proc/stat
#define LINE_MAX_LENGHT 300
// Identyficator for lines with needed information about cpu
#define CPU_ID "cpu"
#define LEN_CPU_ID 3
// Definitions for information when function finished work properly or not
#define SUCCESS 0
#define FAIL 1
// Definitions for threads - was finished or not
#define END_THREAD 0
#define WORKING 1
// Definitions for threads iteration
#define ALL_THREADS 5
#define NO_THREADS 0

#endif
