/****************************************
 * C-ploration 3 for CS 271
 * 
 * [NAME] ERIC ORR
 * [TERM] FALL 2024
 * 
 ****************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 200

 
int main(int argc, const char *argv[])
{	
    // if incorrect number of arguments
    if(argc < 2){
    printf("Usage: %s [filename]\n", argv[0]);
    exit(EXIT_FAILURE);
    }

    FILE *fin = fopen( argv[1], "r" );

    if(fin == NULL){
    perror("Unabled to open file!");
    exit(EXIT_FAILURE);
    }
    char out_file[MAX_LINE_LENGTH];
    strcpy(out_file, argv[1]);
    strcat(out_file, ".echo");

    FILE *fout = fopen(out_file, "w+");

    char line[MAX_LINE_LENGTH] = {0};
    unsigned int line_num = 0;

    while (fgets(line, sizeof(line), fin)){
    line_num++;
    printf("[%04d] %s",line_num,line );
    fprintf(fout,"%s", line);
    }

    fclose(fin);
    fclose(fout);
    return 1;
}
