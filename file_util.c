#include "file_util.h"
#include <stdlib.h>


int file_size(FILE * f)
{
    int pos = ftell(f);
    fseek(f, 0 ,SEEK_END);
    int size = ftell(f);
    fseek(f, pos, SEEK_SET);
    return size;
}

char * read_text_file(const char * filename)
{
    FILE * f = fopen(filename, "r");
    if (f == NULL)
        return NULL;

    int size = file_size(f);
    char * text = (char*)malloc(size + 1);
    fread(text, size, 1, f);
    text[size] = '\0';
    fclose(f);
    return text;
}
