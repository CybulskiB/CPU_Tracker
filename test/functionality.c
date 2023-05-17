#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../headers/global.h"
#include "../headers/reader.h"
#include "../headers/analyzer.h"
#include "../headers/buffer.h"
#include "../headers/printer.h"
#include "functionality_3CPU.h"


int main()
{
 
    int i = 1;
    double exp_results[TESTS_NO][CPU_NO] = EXPECTED_RESULTS;
    char* test_contexts  = (char *) malloc(SP_TESTS_CONTEXTS_SIZE * sizeof(char));
    strcpy(test_contexts,SP_TESTS_CONTEXTS);
    char* test_contexts_pos = test_contexts;
    char* current_context; //During tokenizing malloc isn't necessary
    current_context = strtok_r(test_contexts,CONTEXTS_SEPARATOR,&test_contexts_pos);
    while (i <= TESTS_NO)
    {

        //Sending test data to reader buffer 
        reader_set_buffer(current_context);
        send_reader_to_buffer();

        //Simulation of analyzer buffer - malloc isn't necessary it is in send_data_to_analyzer
        char* test_analyzer_buffer = NULL; //(char *) malloc(MAX_SP_TEST_CONTEXTS_SIZE * sizeof(char ));
        send_data_to_analyzer(&test_analyzer_buffer);

        //Checking connection reader -> buffer -> analyzer 
        assert(0 == strcmp(current_context,test_analyzer_buffer));
        free(test_analyzer_buffer);

        //Real analyzer getting data- analyzing and sending result to buffer
        get_reader_data_from_buffer();
        delete_data_from_reader();
        analyze_data();
        send_analyzer_to_buffer();

        //Simulation of printer
        int test_cpu_no; 
        double test_printer_buffer[PRINTER_CPU_NO];

        send_data_to_printer((double **) &test_printer_buffer, &test_cpu_no);
        free_analyzer_buffer();
        assert(test_cpu_no == CPU_NO );
        
        for(int j =0; j< CPU_NO;j++)
        {
            double buffer_after_round = (int) (test_printer_buffer[j+1] * 100 +0.5)/100.00;
            assert(exp_results[i-1][j] == buffer_after_round);
        } 
        
        printf("Test %d : Passed\n",i);

        i++;
        current_context = strtok_r(test_contexts_pos,CONTEXTS_SEPARATOR,&test_contexts_pos);
        if( i <= TESTS_NO && current_context == NULL )
        {
            printf("Error during functionality test: empty current_context");
        }
    }
    free(test_contexts);

}