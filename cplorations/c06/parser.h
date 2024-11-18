/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] ERIC ORR
 * [TERM] FALL 2024
 * 
 ****************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH MAX_LINE_LENGTH-2

//types
typedef int16_t hack_addr;
typedef int16_t opcode;

enum instruction_type{
    Invalid_Instruction =-1,
    A_Type_Instruction,
    C_Type_Instruction,
};

typedef struct c_instruction {
    opcode a:1;
    opcode comp:6;
    opcode dest:3;
    opcode jump:3;
} c_instruction;

typedef struct a_instruction {
    bool is_addr;
    union a_inst_data{
        hack_addr address;
        char* label;
    };
} a_instruction;

typedef struct instruction {
    union instr_type{
        a_instruction a_instr;
        c_instruction c_instr;
    };
} instruction;

/** function prototypes **/
char *strip(char *s);

void parse(FILE * file);

bool is_Atype(const char*);
bool is_label(const char*);
bool is_Ctype(const char*);

char *extract_label(const char*, char*);