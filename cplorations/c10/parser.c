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
void parse(FILE *file) {
    char line[MAX_LINE_LENGTH] = {0};
    unsigned int line_number = 0;
    unsigned int instruction_number = 0;
    
    add_predefined_symbols(); 
    while (fgets(line, sizeof(line), file)) {
        char inst_type;
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
            inst_type = 'L';
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
        }

        if (is_Atype(line)) {
            if(!parse_A_instruction(line, &instr.instr.a_instr)){
                exit_program(EXIT_INVALID_A_INSTR, line_number, line);
            }
            instr.instr_type = A_Type_Instruction;
        } else if (is_Ctype(line)) {
            inst_type = 'C';
        }

        //printf("%u: %c  %s\n", instruction_number, inst_type, line);
        instruction_number++;
    }
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
            instr->is_addr = true;
            instr->a_inst_data.address = predefined_symbols[i].address;
            free(s);
            return true;
        }
    }

    char* s_end = NULL;
    long result = strtol(s, &s_end,10);

    if(s == s_end){
        //No numbers found, we have a valid string
        instr->is_addr = false;
        //sizeof char isnt needed cause a char is 1 byte, but I think its good practice to have it
        instr->a_inst_data.label = malloc(strlen(s)*sizeof(char) + 1);
        strcpy(instr->a_inst_data.label, s);
    } else if(*s_end != 0){
        //invalid instruction
        free(s);
        return false;
    } else{
        //number found
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
    
    int a;
    instr->jump = jump ? str_to_jumpid(jump) : JMP_NULL;
    instr->dest = dest ? str_to_destid(dest) : DEST_NULL;
    instr->comp = str_to_compid(comp, &a);
    instr->a = a; //some bit-field issue, this gets around it
}

#endif
