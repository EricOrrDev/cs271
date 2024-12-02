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
#include "symtable.h"
#include "error.h"
#include "hack.h"
#include <stdlib.h>

#define MAX_HACK_ADDRESS INT16_MAX
#define MAX_INSTRUCTIONS MAX_HACK_ADDRESS
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
    opcode comp:7;
    opcode dest:4;
    opcode jump:4;
} c_instruction;

typedef struct a_instruction {
    bool is_addr;
    union{
        hack_addr address;
        char* label;
    } a_inst_data;
} a_instruction;

typedef struct instruction {
    union{
        a_instruction a_instr;
        c_instruction c_instr;
    }instr;
    enum instruction_type instr_type;
} instruction;

/** function prototypes **/
char *strip(char *s);

int parse(FILE * file, instruction* instructions);

bool is_Atype(const char*);
bool is_label(const char*);
bool is_Ctype(const char*);

char *extract_label(const char*, char*);

void add_predefined_symbols();

bool parse_A_instruction(const char *line, a_instruction *instr);

void parse_C_instruction(char* line, c_instruction *instr);
