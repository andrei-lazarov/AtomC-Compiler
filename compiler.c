#include  <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include  <ctype.h>

#include "token.h"  // token structure
#include "lexicalAnalyzer.h"
#include "syntacticAnalyzer.h"

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

        crtTk = tokens;
    }


   return 0;
}
