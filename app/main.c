//Includes from standard library
#include <stdio.h>

//Includes from my headers
#include "../headers/analyzer.h"
#include "../headers/global.h"
#include "../headers/reader.h"
#include "../headers/buffer.h"
#include "../headers/printer.h"

int main()
{
    
    read_data();
    send_reader_to_buffer();
    get_reader_data_from_buffer();
    delete_data_from_reader();
    analyze_data();
    send_analyzer_to_buffer();
    free_analyzer_buffer();
    get_analyzer_data_from_buffer();
    print_data();

    return 0;
}