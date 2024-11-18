/****************************************
 * C-ploration 6 for CS 271
 * 
 * [NAME] $YOUR_NAME$
 * [TERM] FALL $YEAR$
 * 
 ****************************************/
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
void parse(FILE * file){
	
    char line[MAX_LINE_LENGTH] = {0};
    unsigned int line_number = 0;
    unsigned int instruction_number = 0;

    while (fgets(line, sizeof(line), file)){
        //char inst_type;
        strip(line);
        if(!*line){
            //jump to next line if empty after strip
            continue;
        }
        
        if(is_label(line)){
            //inst_type = 'L';
            char label[MAX_LABEL_LENGTH];
            extract_label(line, label);
            strcpy(line, label);
            symtable_insert(line,instruction_number);
        } else if(is_Atype(line)){
            //inst_type = 'A';
            instruction_number++;
        } else if(is_Ctype(line)){
           //inst_type = 'C';
           instruction_number++;
        }
        //printf("%c  %s\n",inst_type, line);
        line_number++;
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


