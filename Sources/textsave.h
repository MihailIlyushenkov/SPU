#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys\stat.h>

struct Text
{
    const char * FileName = NULL;
    size_t NumberOfLines = 0;
    char ** TextPivots = NULL;
    char * Buffer = NULL;
};

ssize_t MyGetline(char **lineptr, size_t *n, FILE *stream);
char *strdup(const char *str);

void ReadTextFromFile(Text* Text, const char * Nameoffile);
void ClearText(Text* Text);
size_t GetTextPivots(Text* Text, size_t nchar);
