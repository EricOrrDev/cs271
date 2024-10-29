/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $YOUR_NAME$
 * [TERM] FALL $YEAR$
 * 
 ****************************************/
#include "parser.h"

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
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

    while (fgets(line, sizeof(line), file)){
        char inst_type;
        strip(line);
        if(!*line){
            continue;
        }
        if(is_Atype(line)){
            inst_type = 'A';
        }
        if(is_label(line)){
            inst_type = 'L';
        }
        if(is_Ctype(line)){
            inst_type = 'C';
        }

        printf("%c  %s\n",inst_type, line);
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
    if(!(inputString[0]== '(')){
        return false;
    }
    int i=0;
    char closingPara = inputString[i];
    char nextChar = inputString[i+1];
    while(closingPara != '\0'){
        if(closingPara == ')' && (nextChar == '\0') ){
            return true;
        }
        closingPara = inputString[i++];
        nextChar = inputString[i+1];
    }
    return false;
}

bool is_Ctype(const char* inputString){
    if( (is_Atype(inputString) == false) && (is_label(inputString) == false) ){
        return true;
    }
    return false;
}
