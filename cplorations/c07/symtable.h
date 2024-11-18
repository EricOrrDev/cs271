#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SYMBOL_TABLE_SIZE 1000

typedef int16_t hack_addr;

struct Symbol{
    //key
    char* name;
    //data
    hack_addr address;
};

int hash(char* str);
struct Symbol* symtable_find(char* key);
void symtable_insert(char* key,hack_addr address);
void symtable_display_table();
void symtable_print_labels();
