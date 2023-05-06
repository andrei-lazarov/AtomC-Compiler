#ifndef SYNTACTIC_ANALYZER_H
#define SYNTACTIC_ANALYZER_H

Token *consumedTk;
Token *crtTk;

int consume(int code)
{
    if(crtTk->code==code){
    consumedTk=crtTk;
    crtTk=crtTk->next;
    return 1;
}
    return 0;
}

#endif
