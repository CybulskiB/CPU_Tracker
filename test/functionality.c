#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/analyzer.h"
#include "../inc/buffer.h"
#include "../inc/global.h"
#include "../inc/printer.h"
#include "../inc/reader.h"
#include "functionality_3CPU.h"

static unsigned int i = FIRST_TEST;
static double exp_results[TESTS_NO][CPU_NO_WITH_ROOT] = EXPECTED_RESULTS;
static char *test_contexts;
static char *test_contexts_pos;
static char *current_context;
static char *test_analyzer_buffer;
static int test_cpu_no;
static double test_printer_buffer[CPU_NO_WITH_ROOT];

int main() {

  test_contexts = (char *)malloc(SP_TESTS_CONTEXTS_SIZE * sizeof(char));
  strcpy(test_contexts, SP_TESTS_CONTEXTS);
  test_contexts_pos = test_contexts;
  current_context =
      strtok_r(test_contexts, CONTEXTS_SEPARATOR, &test_contexts_pos);
  init_buffer();
  while (i < TESTS_NO) {

    // Sending test data to reader buffer
    reader_set_buffer(current_context);
    send_reader_to_buffer();

    // Simulation of analyzer buffer
    test_analyzer_buffer = NULL;
    send_data_to_analyzer(&test_analyzer_buffer);
    free_reader_buffer();

    // Checking connection reader -> buffer -> analyzer
    assert(SUCCESS == strcmp(current_context, test_analyzer_buffer));
    free(test_analyzer_buffer);
    reader_set_buffer(current_context);
    send_reader_to_buffer();
    free_reader_buffer();

    // Real analyzer getting data- analyzing and sending result to buffer
    get_reader_data_from_buffer();
    analyze_data();
    send_analyzer_to_buffer();

    // Simulation of printer

    send_data_to_printer((double **)&test_printer_buffer, &test_cpu_no);
    free_analyzer_buffer();
    assert(test_cpu_no == CPU_NO);

    for (size_t j = FIRST_CPU; j <= CPU_NO; j++) {
      double buffer_after_round =
          (int)(test_printer_buffer[j] * ROUND_FORMULA_100 + ROUND_FORMULA_05) /
          TO_PERCENT;
      assert(buffer_after_round >= exp_results[i][j] &&
             exp_results[i][j] >= buffer_after_round);
    }

    printf("Test %d : Passed\n", i + TO_PRETTY_PRINT);

    i++;
    current_context =
        strtok_r(test_contexts_pos, CONTEXTS_SEPARATOR, &test_contexts_pos);
    if (i < TESTS_NO && current_context == NULL) {
      printf("Error during functionality test: empty current_context");
    }
  }
  free(test_contexts);
  destroy_buffer();
  return SUCCESS;
}
