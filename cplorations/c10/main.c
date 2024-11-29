/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] ERIC ORR
 * [TERM] FALL 2024
 * 
 ****************************************/
#include "parser.h"

int main(int argc, const char *argv[])
{		

    if(argc != 2){
        exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);
    }

    FILE *fin = fopen( argv[1], "r" );
    if(fin == NULL){
        exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
    }

    if(fin == NULL){
    perror("Unabled to open file!");
    exit(EXIT_FAILURE);
    }
    
    parse(fin);
    symtable_print_labels();
    fclose(fin);
}


