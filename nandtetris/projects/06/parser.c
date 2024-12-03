/****************************************
 * C-ploration 6 for CS 271
 * 
 * [NAME] ERIC ORR
 * [TERM] FALL 2024
 * 
 ****************************************/
#ifndef __PARSE_H__
#define __PARSE_H__
#include "parser.h"
#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH MAX_LINE_LENGTH-2

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 ****/
char *strip(char *s){	
    char s_new[MAX_LINE_LENGTH];

    int i = 0;
    for (char *s2 = s; *s2; s2++){
        if( *s2 == '/' && *(s2+1) == '/'){
            break;
        } else if(!isspace(*s2)){
            s_new[i++] = *s2;
        }
    }
    s_new[i] = '\0';

    strcpy(s, s_new);
	
    return s;	
}

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments. 
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
int parse(FILE *file, instruction* instructions) {
    char line[MAX_LINE_LENGTH] = {0};
    unsigned int line_number = 0;
    unsigned int instruction_number = 0;
    
    add_predefined_symbols(); 
    while (fgets(line, sizeof(line), file)) {
        instruction instr;
        line_number++;
        if (instruction_number > MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
        }
        strip(line);

        if (!*line) {
            //Skip empty lines after stripping.
            continue;
        }

        if (is_label(line)) {
            char label[MAX_LABEL_LENGTH];
            extract_label(line, label);

            if (!isalpha(label[0])) {
                exit_program(EXIT_INVALID_LABEL, line_number, label);
            }

            if (symtable_find(label) != NULL) {
                exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_number, label);
            }
            symtable_insert(label, instruction_number);
            continue;
        }else if (is_Atype(line)) {
            if(!parse_A_instruction(line, &instr.instr.a_instr)){
                exit_program(EXIT_INVALID_A_INSTR, line_number, line);
            }
            instr.instr_type = A_Type_Instruction;

            if(instr.instr.a_instr.is_addr){
            //if address print the address
            printf("A: %d\n", instr.instr.a_instr.a_inst_data.address);
            } else {
            //else print label
            printf("A: %s\n", instr.instr.a_instr.a_inst_data.label);
            }
        } else if (is_Ctype(line)) {
            char tmp_line[MAX_LINE_LENGTH];
            strcpy(tmp_line, line);
            //printf("DEBUG: pushing string: %s into the parse_C function\n" , tmp_line);
            parse_C_instruction(tmp_line, &instr.instr.c_instr);
            instr.instr_type= C_Type_Instruction;
            if(instr.instr.c_instr.jump == JMP_INVALID){
                exit_program(EXIT_INVALID_C_JUMP, line_number, line);
            }
            if(instr.instr.c_instr.dest == DEST_INVALID){
                exit_program(EXIT_INVALID_C_DEST, line_number, line);
            }
            if(instr.instr.c_instr.comp == COMP_INVALID){
                exit_program(EXIT_INVALID_C_COMP, line_number, line);
            }

            int d = instr.instr.c_instr.dest;
            int c = instr.instr.c_instr.comp;
            int j = instr.instr.c_instr.jump;
            printf("C: d=%d, c=%d, j=%d\n", d, c, j);
        }

        instructions[instruction_number++] = instr;
}
    return instruction_number;
}

bool is_Atype(const char* inputString){
    if (inputString[0] == '@'){
        return true;
    } else {
        return false;
    }
}

bool is_label(const char* inputString){
    if(inputString[0]!= '('){
        return false;
    }
    int i=1;
    while(inputString[i] != ')' && inputString[i+1] != '\0'){
        i++;
    }
    return inputString[i] == ')' && inputString[i+1] == '\0';
}

bool is_Ctype(const char* inputString){
    if( (is_Atype(inputString) == false) && (is_label(inputString) == false) ){
        return true;
    }
    return false;
}

char* extract_label(const char* line, char* label){
    int i=0;
    while(line[i] != ')' && line[i] != '\0'){
        i++;
    }

    strncpy(label,line+1,i-1);
    label[i-1] = '\0';
    return label;
}

void add_predefined_symbols(){
    for(int i =0; i < NUM_PREDEFINED_SYMBOLS; i++){
        //the char* casting is to explicitly say we are no longer const, to get rid of a warning
        symtable_insert( (char*)predefined_symbols[i].name, predefined_symbols[i].address); 
    }
}

bool parse_A_instruction(const char *line, a_instruction *instr){
    char* s = malloc(strlen(line)*sizeof(char));
    strcpy(s, line+1);
    
    //check if a predefined symbol
    for(int i = 0; i <NUM_PREDEFINED_SYMBOLS; i++){
        //strcmp is so weird, why does 0 mean they match?
        if(strcmp(s, predefined_symbols[i].name) == 0){
            instr->a_inst_data.label = predefined_symbols[i].name;
            instr->is_addr = false;
            free(s);
            return true;
        }
    }

    char* s_end = NULL;
    long result = strtol(s, &s_end,10);

    if(s == s_end){
        //No numbers found, we have a valid string, meaning we have a label
        instr->is_addr = false;
        //sizeof char isnt needed cause a char is 1 byte, but I think its good practice to have it
        instr->a_inst_data.label = malloc(strlen(s)*sizeof(char) + 1);
        strcpy(instr->a_inst_data.label, s);
    } else if(*s_end != 0){
        //invalid instruction
        free(s);
        return false;
    } else{
        //number found meaning we have an address 
        instr->is_addr = true;
        instr->a_inst_data.address = result;
    }
    
    free(s);
    return true;
}

void parse_C_instruction(char* line, c_instruction *instr){
    char* token;
    char* jump;
    char* dest;
    char* comp;

    token = strtok(line, ";");
    //I used stack overflow for this, cause the fuck does it mean to look for delimiter on a null string?
    //uses a static pointer I guess? The ergonomics just seems so bad.
    jump = strtok(NULL, ";");

    //token holds comp and destination now
    dest = strtok(token, "=");
    comp = strtok(NULL, "=");

    if(comp == NULL){
        comp = dest;
        dest = NULL;
    }

    //printf("DEBUG: dest='%s', comp='%s', jump='%s'\n",
    //    dest ? dest : "NULL",
    //    comp ? comp : "NULL",
    //    jump ? jump : "NULL");
    
    int a;
    instr->jump = jump ? str_to_jumpid(jump) : JMP_NULL;
    instr->dest = dest ? str_to_destid(dest) : DEST_NULL;
    instr->comp = str_to_compid(comp, &a);
    instr->a = a; //some bit-field issue, this gets around it
    
    //printf("DEBUG: Parsed C-instruction: dest=%d, comp=%d, jump=%d, a=%d\n",
    //       instr->dest, instr->comp, instr->jump, instr->a);
}

opcode instruction_to_opcode(c_instruction instr){
    opcode op = 0;
    op |= (7 << 13);
    op |= (instr.a << 12);
    op |= (instr.comp << 6);
    op |= (instr.dest << 3);
    op |= (instr.jump << 0);
    return op;
}

void assemble(const char * file_name, instruction* instructions, int num_instructions){
    const char* suffix = ".hack";
    int lastVariableIndex = 16;
    
    FILE* fin = fopen( file_name, "w");
    
    for(int i = 0; i < num_instructions; i++){
        opcode operationCode;
        if(instructions[i].instr_type == A_Type_Instruction){
            //if A type and Label
            if(instructions[i].instr.a_instr.is_addr == false){
                struct Symbol* symbol = symtable_find(instructions[i].instr.a_instr.a_inst_data.label);
                if( symbol != NULL){
                    //if label is found in symbol table set opcode as address
                    operationCode = symbol->address;
                } else {
                    //if label is not found, add it to symbol table, starting at 16
                    //i think symtable is the wrong function for this, we need to specify its exact location, not hash it
                    symtable_insert(instructions[i].instr.a_instr.a_inst_data.label, lastVariableIndex);
                    lastVariableIndex++;
                    //set opcode as address of new label
                    symbol = symtable_find(instructions[i].instr.a_instr.a_inst_data.label);
                    operationCode = symbol->address;
                }
                free(instructions[i].instr.a_instr.a_inst_data.label);
            } else{
                //if A type and address
                operationCode = instructions[i].instr.a_instr.a_inst_data.address;
            }
        }

        if(instructions[i].instr_type == C_Type_Instruction){
            //look up opcode
            operationCode = instruction_to_opcode(instructions[i].instr.c_instr);
        }
        fprintf("%c", OPCODE_TO_BINARY(operationCode));
    }
    
    printf("DEBUG - parser.c assemble: assembling %s complete", file_name);
    fclose(fin);
}
#endif


