#include <stdio.h>
#include <string.h>
#include "commands.h"

int main()
{
    FILE* ComAssFile;
    ComAssFile = fopen("TextFiles/CommandAssemblyFile.txt", "r");

    if (ComAssFile == NULL)
    {
        printf("\n\nCommand File not open\n\n");
        return 0;
    }

    FILE* DebugFile;
    DebugFile = fopen("TextFiles/DisassembledCommandFile.txt", "w");

    int word1 = 0;
    int word2 = 0;

    int flag = 1;

    while (fscanf(ComAssFile, "%d", &word1) == 1)
    {
        flag = 0;
        switch(word1){
            case hlt: fprintf(DebugFile, "hlt\n"); break;
            case push: fprintf(DebugFile, "push "); (fscanf(ComAssFile, "%d", &word2) == 1)?fprintf(DebugFile, "%d\n", word2):fprintf(DebugFile, "invalid data"); break;
            case add: fprintf(DebugFile, "add\n"); break;
            case sub: fprintf(DebugFile, "sub\n"); break;
            case mul: fprintf(DebugFile, "mul\n"); break;
            case div_: fprintf(DebugFile, "div_\n"); break;
            case sqrt_: fprintf(DebugFile, "sqrt_\n"); break;
            case out: fprintf(DebugFile, "out\n"); break;
            case in: fprintf(DebugFile, "in\n"); break;
            default: fprintf(DebugFile, "Invalid command, code is shit"); return 0;
        }
    }

    if (flag)
    {
        fprintf(DebugFile, "cant read commands from file((");
    }

    return 0;
}
