#include "lexer.h" 

FILE *fp;
static const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct", "else if", "printf", "scanf"
}; 

int curl_brac_flag, round_brac_flag;
extern char ok_flag;
extern int line_no;
static const char* operators = "+-*/%=!|&<>^";
static const char* specialCharacters = ",;{}()[]:";
static char div_flag=0;
int initializeLexer(const char* filename)
{
    if(strstr(filename, ".c"))
    {
        fp = fopen(filename, "r");
        if(fp==NULL) 
        {
            printf("ERROR: Unable to open %s\n", filename);
            return 0;
        }
        else 
        {
            printf("Open    :   %s  :  Success\n", filename);
            printf("Parsing :   %s  :  Started\n", filename);
            return 1;
        }
    }
    else
    {
        printf("ERROR: Input file is not C file\n");
        return 0;
    }
}

Token getNextToken()  //do not forget to add about division operator
{
    // int idx = 0;
    static char ch;
    Token token;
    token.type = UNKNOWN;
    while((ch = fgetc(fp))!=EOF)
    {
        //skip #include files
        if(ch=='#')
        {
            while((ch=fgetc(fp))!='\n');
        }
        //skip single line comments
        else if(ch=='/' && !div_flag)
        {
            ch = fgetc(fp);
            if(ch=='/')
            {
                while(ch=fgetc(fp)!='\n');
                line_no++;
            }
            //skip multi-line comments 
            else if(ch=='*')
            {
                while((ch=fgetc(fp))!='*')
                {
                    if(ch=='\n') line_no++;
                }
                ch = fgetc(fp);
                while(ch!='/')
                {                
                    if(ch=='\n') line_no++;
                    while((ch=fgetc(fp))!='*')
                    {
                        if(ch=='\n') line_no++;
                    }
                    ch = fgetc(fp);
                }
            }
            else
            {
                div_flag = 1;
                fseek(fp, ftell(fp)-2, SEEK_SET);
                continue;
            }
            
        }
        //break for operator
        else if(isOperator(ch))
        {
            int i=0;
            token.lexeme[i++] = ch;
            if(isOperator(ch=fgetc(fp)) && (ch!='!') && (ch!='%' && (ch!='^') && (ch!='*'))) 
            {
                token.lexeme[i++] = ch;
            }
            else
            {
                fseek(fp, ftell(fp)-1, SEEK_SET);
            }
            token.lexeme[i] = '\0';
            token.type = OPERATOR;
            strcpy(token.lex_type, "Operater");
            break;
        }
        // break for special character
        else if(isSpecialCharacter(ch))
        {
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            token.type = SPECIAL_CHARACTER;
            strcpy(token.lex_type, "Special character");
            break;
        }      
        else if(ch=='"')
        {
            int i=0;
            token.lexeme[i++] = ch;
            while (1)
            {
                ch=fgetc(fp);
                if(ch=='\n') 
                {
                    ok_flag = 0;
                    printf("ERROR: Missing double quote in line %d\n", line_no+1);
                    return token;
                }
                if(ch=='"')
                {
                    break;
                }
                token.lexeme[i++] = ch;
            }
            token.lexeme[i++] = ch;
            token.lexeme[i] = '\0';
            categorizeToken(&token);
            break;
            
        }
        else if(ch=='0')  //for binary, hex and octal numbers
        {
            int i=0;
            token.lexeme[i++] = ch;
            while (1)
            {
                ch=fgetc(fp);
                if(! (isdigit(ch) || (ch>='a' && ch<='z') || (ch>='A' && ch<='Z') ))break;
                token.lexeme[i++] = ch;
            }
            token.lexeme[i] = '\0';
            fseek(fp, ftell(fp)-1, SEEK_SET);
            int num_sys = ishex_bin_oct(token.lexeme);
            if(num_sys==NOT_HEX)
            {
                ok_flag = 0;
                printf("ERROR: %s is not a valid hexadecimal number\n", token.lexeme);
                return token;
            }
            else if(num_sys==NOT_BIN)
            {
                ok_flag = 0;
                printf("ERROR: %s is not a valid binary number\n", token.lexeme);
                return token;
            }
            else if(num_sys==NOT_OCT)
            {
                ok_flag = 0;
                printf("ERROR: %s is not a valid octal number\n", token.lexeme);
                return token;
            }
            else
            {
                strcpy(token.lex_type, "Constant");
                token.type = CONSTANT;
                break;
            }
        }

        else if(isdigit(ch) || (ch>='a' && ch<='z') || ch=='_' || (ch>='A' && ch<='Z'))
        {
            int i=0;
            token.lexeme[i++] = ch;
            while (1)
            {
                ch=fgetc(fp);
                if(!(isdigit(ch) || (ch>='a' && ch<='z') || ch=='_' || (ch>='A' && ch<='Z') || ch=='.'))break;
                token.lexeme[i++] = ch;
            }
            token.lexeme[i] = '\0';
            fseek(fp, ftell(fp)-1, SEEK_SET);
            // printf("lex=%s\n", token.lexeme);
            categorizeToken(&token);
            break;
        } 
        else if(ch=='\'')  //for character constant char ch = 'a'
        {
            int i=0;
            token.lexeme[i++] = ch; 
            while (1)
            {
                ch=fgetc(fp);
                if(ch=='\n')
                {
                    ok_flag = 0;
                    printf("ERROR: Missing single quote in line %d\n", line_no+1);
                    return token;
                }
                if(ch=='\'') 
                {
                    break;
                }
                token.lexeme[i++] = ch;
            }
            token.lexeme[i++] = '\'';
            token.lexeme[i] = '\0'; 
            if(strlen(token.lexeme)==3) 
            {
                token.type = CONSTANT;
                strcpy(token.lex_type, "Constant");
                break;
            }
            else
            {
                token.type = UNKNOWN;
                strcpy(token.lex_type, "Unknown");
                break;
            }
        }

        if(ch=='\n')
        {
            line_no++;
            if(round_brac_flag>0) 
            {
                ok_flag=0;
                printf("WARNING: missing ) in line %d", line_no);
                token.type = UNKNOWN;
                break;
            }
            if(round_brac_flag<0) 
            {
                ok_flag=0;
                printf("WARNING: missing ( in line %d", line_no);
                token.type = UNKNOWN;
                break;
            }  
        }
    }

    if(feof(fp))
    {
        if(curl_brac_flag>0)
        {
            printf("WARNING: missing }");
            ok_flag=0;
        }
        if(curl_brac_flag<0)
        {
            printf("WARNING: missing {");
            ok_flag=0;
        } 

    }  
    return token;

}

void categorizeToken(Token* token)
{
    if(isKeyword(token->lexeme))
    {
        token->type = KEYWORD;
        strcpy(token->lex_type, "Keyword");
        return ;
    }
    if(isIdentifier(token->lexeme))
    {
        token->type = IDENTIFIER;
        strcpy(token->lex_type, "Identifier");
        return ;
    }
    if(isConstant(token->lexeme))
    {
        token->type = CONSTANT;
        strcpy(token->lex_type, "Constant");        
        return ;
    }
    else 
    {
        token->type = UNKNOWN;
        strcpy(token->lex_type, "Unknown");
        return ;
    }
}

int isKeyword(const char* str)
{
    for(int i=0; i<MAX_KEYWORDS; i++)
    {
        if(!strcmp(str, keywords[i]))
            return 1;
    }
    return 0;
}


int isOperator(char ch)
{
    int i=0;
    if(ch=='/') div_flag=0;
    while(operators[i]!='\0')
    {
        if(ch==operators[i++])
            return 1;
    }
    return 0;
}

int isSpecialCharacter(char ch)
{
    int i=0;
    if(ch=='{') curl_brac_flag++;
    if(ch=='}') curl_brac_flag--;
    if(ch=='(') round_brac_flag++;
    if(ch==')') round_brac_flag--;
    
    while(specialCharacters[i]!='\0')
    {
        if(ch==specialCharacters[i++])
            return 1;
    }
    return 0;
}

int isConstant(const char* str)
{
    char dot_flag=0;
    int i=0;
    if(str[0]=='"' && str[strlen(str)-1]=='"')
        return 1;

    while(str[i]!='\0')
    {
        if(str[i]=='.') 
        {
            i++; 
            dot_flag=1;
            continue;
        }
        if(!(isdigit(str[i++]) || (dot_flag && ((i==strlen(str) && (str[i-1]=='f' || str[i-1]=='F')))))) 
            return 0;
    }
    
    return 1;

}

int isIdentifier(const char* str)
{
    int i = 0;
    if(str[0]=='_' || (str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z'))
        return 1;
    return 0;
}


int ishex_bin_oct(const char* str)
{
    int i=2;
    // puts("hi 0\n");
    if(str[1] == 'x')
    {
        // printf("hi\n");
        while (str[i])
        {
            if(!( isdigit(str[i]) || (str[i]>='a' && str[i]<='f') || (str[i]>='A' && str[i]<='F') ))
                return NOT_HEX;
            i++;
        }
        return 1;
    }
    else if(str[1] == 'b')
    {
        while (str[i])
        {
            
            if(!(str[i]=='1' || str[i]=='0'))
                return NOT_BIN;
            i++;
        }
        return 1;
    }
    else
    {
        i=1;
        while (str[i])
        {
            if(!(str[i]>='0' && str[i]<='7'))
                return NOT_OCT;
            i++;
        }
        return 1;
    }
}