#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_KEYWORDS        23
#define MAX_TOKEN_SIZE      100
#define LEX_TYPE_SIZE       25
#define NOT_HEX             0
#define NOT_BIN             -1
#define NOT_OCT             -2

typedef enum {
    KEYWORD, 
    OPERATOR,
    REL_OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    IDENTIFIER,
    UNKNOWN
} TokenType;

typedef struct {
    char lexeme[MAX_TOKEN_SIZE];
    char lex_type[LEX_TYPE_SIZE];
    TokenType type;
} Token; 

int initializeLexer(const char* filename);
Token getNextToken();
void categorizeToken(Token* token);
int isKeyword(const char* str);
int isOperator(char ch);
int isSpecialCharacter(char ch);
int isConstant(const char* str);
int isIdentifier(const char* str);
int ishex_bin_oct(const char* str);



#endif
