#include  <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include  <ctype.h>

#include "lib/token.h"  
#include "lib/errors.h"
#include "lib/input.h"
#include "lib/lexicalAnalyzer.h"
//#include "lib/syntacticAnalyzer.h"

int main(int argc, char *argv[]) {
   
    checkArguments(argc);
    char *fileContent = getFile(argv[1]);
    lexicalAnalysis(fileContent, "stdout");

    //crtTk = tokens;
    
    freeTokens();
    return 0;
}
