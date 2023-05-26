#ifndef INPUT_H
#define INPUT_H

void checkArguments(int argc) {
    if (argc != 2)
        err("Wrong arguments. Usage: ./compiler input.txt\n");
}

char* getFile(char *path){

    char *content;
    long length;
    FILE *pointer = fopen (path, "r");

    if(pointer == NULL)
        err("Input file not found.\n");
    
    fseek (pointer, 0, SEEK_END);
    length = ftell (pointer);
    fseek (pointer, 0, SEEK_SET);
    content = malloc (length+1);
    if (content) {
        fread (content, 1, length, pointer);
        content[length]=0;
    }
    fclose (pointer);

    return content;
}

#endif
