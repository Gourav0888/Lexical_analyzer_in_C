#include <stdio.h>
#include "lexer.h"

extern FILE *fp;
char ok_flag=1;
int line_no = 0;
int main(int argc, char *argv[]) 
{
    // initializeLexer(argv[1]);
    
    if(argc!=2)
    {
        printf("ERROR: Insufficient number of arguments\n");
        return 0;
    }
    

    if(!initializeLexer(argv[1]))
    {
        return 0;
    }
    
    Token token;
    while ((token = getNextToken()).type != UNKNOWN) 
    {
        printf("%-20s: %s\n", token.lex_type, token.lexeme);
    }
    // printf("feof=%d\n", feof(fp));
    if(!feof(fp) && ok_flag)  //file not ended, 
    {
        printf("\nUnknown: %s, in line %d\n", token.lexeme, line_no+1);
    }
    if(feof(fp) && ok_flag)    //all okay
    {
        printf("\n\nParsing :   %s  :  Done\n\n", argv[1]);
    }
    else
    {
        printf("\n\nParsing :   %s  :  Failed\n\n", argv[1]);
    }

    return 0;
}
