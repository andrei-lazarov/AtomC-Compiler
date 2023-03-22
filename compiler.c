#include  <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include  <ctype.h>
#include <string.h>

#define SAFEALLOC(var,Type) if((var=(Type*)malloc(sizeof(Type)))==NULL)err("not enough memory");

enum{ID, END, CT_INT, ASSIGN, EQUAL, BREAK, CHAR, DOUBLE, ELSE, FOR, IF, INT, RETURN, STRUCT, VOID, WHILE, CT_REAL, CT_CHAR, CT_STRING, COMMA, SEMICOLON, LPAR, RPAR, LBRACKET, RBRACKET, LACC, RACC, ADD, SUB, MUL, DIV, DOT, AND, OR, NOT, NOTEQ, LESS, LESSEQ, GREATER, GREATEREQ}; // tokens codes

typedef struct _Token {
    int code; // code (name)
    union{
        char        *text; // used for ID, CT_STRING (dynamically allocated)
        long int    i; // used for CT_INT, CT_CHAR
        double      r; // used for CT_REAL
    };
    int line; // the input file line
    struct _Token *next; // link to the next token
}Token;

Token *lastToken = NULL;
Token *tokens = NULL;
char *pCrtCh = 0;
int line;


char *createString (const char *start, const char *end) {
    // Calculate the length of the substring to copy
    size_t len = end - start;

    // Allocate memory for the new string
    char* newStr = malloc((len + 1) * sizeof(char));

    // Copy the substring into the new string
    memcpy(newStr, start, len);
    newStr[len] = '\0';

    // Return the new string
    return newStr;
}


void err(const char *fmt,...) {
    va_list va;
    va_start(va,fmt);
    fprintf(stderr,"error: ");
    vfprintf(stderr,fmt,va);
    fputc('\n',stderr);
    va_end(va);
    exit(-1);
}

void tkerr(const Token *tk,const char *fmt,...) {
    va_list va;
    va_start(va,fmt);
    fprintf(stderr,"error in line %d: ",tk->line);
    vfprintf(stderr,fmt,va);
    fputc('\n',stderr);
    va_end(va);
    exit(-1);
}

Token *addTk(int code) {
    Token *tk;
    SAFEALLOC(tk,Token);
    tk->code=code;
    tk->line=line;
    tk->next=NULL;
    if(lastToken){
        lastToken->next=tk;
    }else{
        tokens=tk;
    }
    lastToken=tk;
    return tk;
}



int getNextToken() {
    int state=0,nCh;
    char ch;
    const char *pStartCh;
    
    Token *tk;

    while(1) { // infinite loop
        ch=*pCrtCh;
        
        switch(state) {
            case 0: // transitions test for state 0
                if(isalpha(ch)||ch=='_'){
                    pStartCh=pCrtCh; // memorizes the beginning of the ID
                    pCrtCh++; // consume the character
                    state=1; // set the new state
                }
                else if(ch=='='){
                    pCrtCh++;
                    state=3;
                }
                else if(ch==' '||ch=='\r'||ch=='\t'){
                    pCrtCh++; // consume the character and remains in state 0
                }
                else if(ch=='\n'){ // handled separately in order to update the current line
                    line++;
                    pCrtCh++;
                }
                else if(ch==0){ // the end of the input string
                    pCrtCh++;
                    addTk(END);
                    return END;
                }
                else if(ch==',') {
                    pCrtCh++;
                    addTk(COMMA);
                    return COMMA;
                }
                else if(ch==';') {
                    pCrtCh++;
                    addTk(SEMICOLON);
                    return SEMICOLON;
                }
                else if(ch=='(') {
                    pCrtCh++;
                    addTk(LPAR);
                    return LPAR;
                }
                else if(ch==')') {
                    pCrtCh++;
                    addTk(RPAR);
                    return RPAR;
                }
                else if(ch=='[') {
                    pCrtCh++;
                    addTk(LBRACKET);
                    return LBRACKET;
                }
                else if(ch==']') {
                    pCrtCh++;
                    addTk(RBRACKET);
                    return RBRACKET;
                }
                else if(ch=='{') {
                    pCrtCh++;
                    addTk(LACC);
                    return LACC;
                }
                else if(ch=='}') {
                    pCrtCh++;
                    addTk(RACC);
                    return RACC;
                }
                else if(ch=='+') {
                    pCrtCh++;
                    addTk(ADD);
                    return ADD;
                }
                else if(ch=='-') {
                    pCrtCh++;
                    addTk(SUB);
                    return SUB;
                }
                else if(ch=='*') {
                    pCrtCh++;
                    addTk(MUL);
                    return MUL;
                }
                else if(ch=='/') {
                    pCrtCh++;
                    addTk(DIV);
                    return DIV;
                }
                else if(ch=='.') {
                    pCrtCh++;
                    addTk(DOT);
                    return DOT;
                }
                else if(ch=='&') {
                    pCrtCh++;
                    if (*pCrtCh=='&') {
                        pCrtCh++;
                        addTk(AND);
                        return AND;
                    }
                    else tkerr(addTk(END),"invalid character");
                }
                else if(ch=='|') {
                    pCrtCh++;
                    if (*pCrtCh=='|') {
                        pCrtCh++;
                        addTk(OR);
                        return OR;
                    }
                    else tkerr(addTk(END),"invalid character");
                }
                else if(ch=='!') {
                    pCrtCh++;
                    if (*pCrtCh=='=') {
                        pCrtCh++;
                        addTk(NOTEQ);
                        return NOTEQ;
                    }
                    else {
                        addTk(NOT);
                        return NOT;
                    }
                }
                
                else if(ch=='<') {
                    pCrtCh++;
                    if (*pCrtCh=='=') {
                        pCrtCh++;
                        addTk(LESSEQ);
                        return LESSEQ;
                    }
                    else {
                        addTk(LESS);
                        return LESS;
                    }
                }

                else if(ch=='>') {
                    pCrtCh++;
                    if (*pCrtCh=='=') {
                        pCrtCh++;
                        addTk(GREATEREQ);
                        return GREATEREQ;
                    }
                    else {
                        addTk(GREATER);
                        return GREATER;
                    }
                }
                else tkerr(addTk(END),"invalid character");
                break;
            case 1:
                if (isalnum(ch)||ch=='_') pCrtCh++;
                else state=2;
                break;
            case 2:
                nCh = pCrtCh - pStartCh; // the id length
                // keywords tests
                if(nCh==5&&!memcmp(pStartCh,"break",5)) tk=addTk(BREAK);
                else if(nCh==4&&!memcmp(pStartCh,"char",4)) tk=addTk(CHAR);
                else if(nCh==6&&!memcmp(pStartCh,"double",6)) tk=addTk(DOUBLE);
                else if(nCh==4&&!memcmp(pStartCh,"else",4)) tk=addTk(ELSE);
                else if(nCh==3&&!memcmp(pStartCh,"for",3)) tk=addTk(FOR);
                else if(nCh==2&&!memcmp(pStartCh,"if",2)) tk=addTk(IF);
                else if(nCh==3&&!memcmp(pStartCh,"int",3)) tk=addTk(INT);
                else if(nCh==6&&!memcmp(pStartCh,"return",6)) tk=addTk(RETURN);
                else if(nCh==6&&!memcmp(pStartCh,"struct",6)) tk=addTk(STRUCT);
                else if(nCh==4&&!memcmp(pStartCh,"void",4)) tk=addTk(VOID);
                else if(nCh==5&&!memcmp(pStartCh,"while",5)) tk=addTk(WHILE);
                // … all keywords …
        
                else{ // if no keyword, then it is an ID
                    tk=addTk(ID);
                    tk->text=createString(pStartCh,pCrtCh);
                }
                return tk->code;
            case 3:
                if(ch=='='){
                    pCrtCh++;
                    state=4;
                }
            else state=5;
            break;
            case 4:
                addTk(EQUAL);
                return EQUAL;
            case 5:
                addTk(ASSIGN);
                return ASSIGN;
            case 6:

        }
    }
}

void showTokens () {
    Token *current = tokens;

    while (current != NULL) {
        printf("%d  ", current->code);
        current = current->next;
    }
}

int main() {
   
    char *fileContent = 0;
    long fileLength;
    FILE *filePointer = fopen ("input.txt", "r");

    if (filePointer) {
        fseek (filePointer, 0, SEEK_END);
        fileLength = ftell (filePointer);
        fseek (filePointer, 0, SEEK_SET);
        fileContent = malloc (fileLength+1);
        if (fileContent) {
            fread (fileContent, 1, fileLength, filePointer);
            fileContent[fileLength]=0;}
        fclose (filePointer);
    }

    if (fileContent) {
        //printf("%s", fileContent);
        pCrtCh = fileContent;
        while (1) {
            int output = getNextToken();
            if (output == END) break; 
        }
        
        showTokens();
        putchar('\n');
    }


   return 0;
}
