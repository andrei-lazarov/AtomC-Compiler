#ifndef SYNTACTIC_ANALYZER_H
#define SYNTACTIC_ANALYZER_H

Token *consumedTk;
Token *crtTk;

int consume(int code) {
    if(crtTk->code==code){
        consumedTk=crtTk;
        crtTk=crtTk->next;
        return 1;
    }
    return 0;
}

int expr() {
    
}

int ruleWhile()
{
    if(!consume(WHILE))return 0;
    if(!consume(LPAR))tkerr(crtTk,"missing ( after while");
    if(!expr())tkerr(crtTk,"invalid expression after (");
    if(!consume(RPAR))tkerr(crtTk,"missing )");
    if(!stm())tkerr(crtTk,"missing while statement");
    return 1;
}

int factor()
{
    Token *startTk=crtTk;
    if(consume(ID))
        return 1;
    if(consume(CT_INT))
        return 1;
    if(consume(LPAR)){
        if(expr()){
            if(consume(RPAR))
                return 1;
            else tkerr(crtTk,"missing )");
        }
        else tkerr(crtTk,"invalid expression after (");
    crtTk=startTk; // restore crtTk to the entry value
    }
    return 0;
}

int typeName()
{
    if(!typeBase())return 0;
    arrayDecl();
    return 1;
}

int stmCompound()
{
    if(!consume(LACC))return 0;
    while(1){
        if(declVar()){
        }
        else if(stm()){
        }
        else break;
    }
    if(!consume(RACC))tkerr(crtTk,"missing } or syntax error");
    return 1;
}

#endif
