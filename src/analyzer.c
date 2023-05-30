// Includes from standard library
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes from my headers
#include "analyzer.h"
#include "buffer.h"
#include "global.h"
#include "watchdog.h"
// Varibles before and after analyzing
static char *analyzer_internal_buffer = NULL;
static double cpu_percent[CPU_MAX];
static int cpu_counter;
// Variables for analyzer formula
static size_t prev_idle[CPU_MAX], prev_iowait[CPU_MAX];
static size_t prev_user[CPU_MAX], prev_nice[CPU_MAX], prev_system[CPU_MAX];
static size_t prev_irq[CPU_MAX], prev_softirq[CPU_MAX], prev_steal[CPU_MAX];
static size_t curr_idle, curr_iowait = CURR_INIT;
static size_t curr_user, curr_nice, curr_system, curr_irq, curr_softirq,
    curr_steal = CURR_INIT;
static size_t PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total;
static size_t totald, idled;
static int analyze_no = FIRST;
// Variable for analyzer thread
static int analyzer_control = WORKING;
// Variable to use during analysing
static char *line_copy;

void *analyzer_task(void *arg) {
  if (arg != NULL) {
    save_logger_data("Warning: Analyzer got unexpected argument");
  }

  while (analyzer_control) {
    confirm_work(ANALYZER_ID);
    get_reader_data_from_buffer();
    analyze_data();
    send_analyzer_to_buffer();
    free_analyzer_buffer();
  }
  return NULL;
}

void analyze_data(void) {

  char *proc_stat_line;
  char *cpu_use_info;
  char *proc_stat_line_pos = analyzer_internal_buffer;

  cpu_counter = START_COUNT_CPU;
  proc_stat_line =
      strtok_r(analyzer_internal_buffer, CPU_ID, &proc_stat_line_pos);
  while (proc_stat_line != NULL) {
    cpu_counter++;
    line_copy = (char *)malloc(sizeof(line_copy) * strlen(proc_stat_line));
    strcpy(line_copy, proc_stat_line);
    cpu_use_info = strtok(line_copy, INFO_SEPARATOR);
    if (cpu_counter != CPU_TOTAL) {
      cpu_use_info = strtok(NULL, INFO_SEPARATOR);
    }
    if (set_curr(cpu_use_info) < GUEST_POS) {
      save_logger_data(
          "Error: Analyzer got too little information in set_curr()");
      stop_analyzer();
      break;
    }

    if (analyze_no) {
      PrevIdle = PREV_INIT;
      PrevNonIdle = PREV_INIT;
      PrevTotal = PREV_INIT;
      analyze_no = NEXT;

    } else {
      PrevIdle = prev_idle[cpu_counter] + prev_iowait[cpu_counter];
      PrevNonIdle = prev_user[cpu_counter] + prev_nice[cpu_counter] +
                    prev_system[cpu_counter] + prev_irq[cpu_counter] +
                    prev_softirq[cpu_counter] + prev_steal[cpu_counter];
      PrevTotal = PrevIdle + PrevNonIdle;
    }
    Idle = curr_idle + curr_iowait;
    NonIdle = curr_user + curr_nice + curr_system + curr_irq + curr_softirq +
              curr_steal;
    Total = Idle + NonIdle;
    totald = Total - PrevTotal;
    idled = Idle - PrevIdle;
    cpu_percent[cpu_counter] =
        TO_PERCENT * (double)(totald - idled) / (double)totald;
    set_prev(cpu_counter);

    proc_stat_line = strtok_r(proc_stat_line_pos, CPU_ID, &proc_stat_line_pos);

    if (cpu_counter > CPU_MAX) {
      save_logger_data("Unexpected high number of cpu in analyze_data()");
    }

    free(line_copy);
  }
}
int set_curr(char *cpu_line) {
  int infos_counter = USER_POS;
  while (cpu_line != NULL) {
    switch (infos_counter) {
    case USER_POS:
      curr_user = strtoul(cpu_line, NULL, NUM_BASE);
      break;
    case NICE_POS:
      curr_nice = strtoul(cpu_line, NULL, NUM_BASE);
      break;
    case SYSTEM_POS:
      curr_system = strtoul(cpu_line, NULL, NUM_BASE);
      break;
    case IDLE_POS:
      curr_idle = strtoul(cpu_line, NULL, NUM_BASE);
      break;
    case IOWAIT_POS:
      curr_iowait = strtoul(cpu_line, NULL, NUM_BASE);
      break;
    case IRQ_POS:
      curr_irq = strtoul(cpu_line, NULL, NUM_BASE);
      break;
    case SOFTIRQ_POS:
      curr_softirq = strtoul(cpu_line, NULL, NUM_BASE);
      break;
    case STEAL_POS:
      curr_steal = strtoul(cpu_line, NULL, NUM_BASE);
      break;
    default:
      break;
    }
    infos_counter++;
    cpu_line = strtok(NULL, INFO_SEPARATOR);
  }
  return infos_counter;
}

void set_prev(int position) {
  if (position > CPU_MAX) {
    save_logger_data("Error: Analyzer got to high position in set prev");
    stop_analyzer();
  } else {
    prev_user[position] = curr_user;
    prev_nice[position] = curr_nice;
    prev_system[position] = curr_system;
    prev_idle[position] = curr_idle;
    prev_iowait[position] = curr_iowait;
    prev_irq[position] = curr_irq;
    prev_softirq[position] = curr_softirq;
    prev_steal[position] = curr_steal;
  }
}
void get_reader_data_from_buffer(void) {
  send_data_to_analyzer(&analyzer_internal_buffer);
}
void send_analyzer_to_buffer() { save_analyzer_data(cpu_percent, cpu_counter); }
void free_analyzer_buffer(void) {
  free(analyzer_internal_buffer);
  analyzer_internal_buffer = NULL;
}
void stop_analyzer(void) { analyzer_control = END_THREAD; }
