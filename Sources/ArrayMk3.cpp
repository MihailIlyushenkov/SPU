#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include "textsave.h"

ssize_t MyGetline(char **lineptr, size_t *n, FILE *stream)
{
    size_t size = 128;
    char *subbuf_p;
    subbuf_p = (char *)calloc(size, sizeof(char));
    if (subbuf_p == NULL)
    {
        return -1;
    }

    char Readedchar = getc(stream);
    size_t counter = 0;

    while (Readedchar != '\n')
    {
        subbuf_p[counter] = Readedchar;
        counter += 1;
        Readedchar = getc(stream);

        if (counter > size-1)
        {
            size += 128;
            subbuf_p = (char *)realloc(subbuf_p, size);

            if (subbuf_p == NULL)
            {
                return -1;
            }
        }
    }

    if ((*lineptr == NULL))
    {
        *n = counter + 1;
        *lineptr = (char*)calloc(counter + 1, sizeof(char));

        if (lineptr == NULL)
        {
            return -1;
        }

        for (size_t i = 0; i < counter; i++)
        {
            (*lineptr)[i] = subbuf_p[i];
        }
        (*lineptr)[counter] = '\0';
    }
    else
    {
        if (*n < size)
        {
            *n = counter;
            *lineptr = (char*)realloc(*lineptr, counter);

            if (lineptr == NULL)
            {
                return -1;
            }

        }
        for (size_t i = 0; i < counter; i++)
        {
            (*lineptr)[i] = subbuf_p[i];
        }
    }

    (*lineptr)[counter] = '\0';
    return counter;

}

char *strdup(const char *str)
{
    int Stringlen = 0;
    while (str[Stringlen] != '\0')
    {
        Stringlen += 1;
    }

    char *NewString = (char*)calloc(Stringlen + 1, sizeof(char));

    if (NewString == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < Stringlen; i++)
    {
        NewString[i] = str[i];
    }

    return NewString;
}

void ClearText(Text* Text)
{
    free(Text->Buffer);
    free(Text->TextPivots);
}

void ReadTextFromFile(Text* Text, const char* Nameoffile)
{
    FILE *file;
    Text->FileName = Nameoffile;

    file = fopen(Text->FileName, "r");

    size_t nchar = 0;

    struct stat FileData;
    stat(Text->FileName, &FileData);

    char ReadedChar = '0';

    nchar = FileData.st_size + 1;


    Text -> Buffer = (char *) calloc(nchar, sizeof(char));

    fread(Text->Buffer, sizeof(char), nchar, file);
    fclose(file);
    GetTextPivots(Text, nchar);
}

size_t GetTextPivots(Text* Text, size_t nchar)
{
    size_t nlines = 0;

    for (size_t i = 0; i < nchar; i++)
    {
        if ( (Text->Buffer)[i] == '\n')
        {
            nlines += 1;
        }
    }
    nlines++;
    Text->NumberOfLines = nlines;
    // printf("%zu\n", Text->NumberOfLines);

    Text->TextPivots = (char **) calloc(nlines, sizeof(char*));
    (Text->TextPivots)[0] = Text->Buffer;

    int StringNum = 1;
    for (size_t i = 0; i < nchar - 1; i++)
    {
        if ((Text->Buffer)[i] == '\n')
        {

            (Text->TextPivots)[StringNum] = &((Text->Buffer)[i+1]);
            StringNum += 1;
            (Text->Buffer)[i] = '\0';
        }
    }
    return nlines;
}
