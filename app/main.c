//Includes from standard library
#include <stdio.h>
#include <pthread.h>
//Includes from my headers
#include "../headers/analyzer.h"
#include "../headers/global.h"
#include "../headers/reader.h"
#include "../headers/buffer.h"
#include "../headers/printer.h"

int main()
{
    init_buffer();
    /*
    read_data();
    send_reader_to_buffer();
    get_reader_data_from_buffer();
    analyze_data();
    send_analyzer_to_buffer();
    free_analyzer_buffer();
    get_analyzer_data_from_buffer();
    print_data();*/
    pthread_t reader, analyzer, printer;

    if(pthread_create(&reader,NULL,reader_task,NULL) != 0)
    {
        perror("Failed to create reader thread");
    }
    if(pthread_create(&analyzer,NULL,analyzer_task,NULL) != 0)
    {
        perror("Failed to create analyzer thread");
    }
    if(pthread_create(&printer,NULL,printer_task,NULL) != 0)
    {
        perror("Failed to create printer thread");
    }


    if(pthread_join(reader,NULL) != 0)
    {
        perror("Failed to join reader thread");
    }
    if(pthread_join(analyzer,NULL) != 0)
    {
        perror("Failed to join analyzer thread");
    }
    if(pthread_join(printer,NULL) != 0)
    {
        perror("Failed to join printer thread");
    }

    destroy_buffer();

    return 0;
}