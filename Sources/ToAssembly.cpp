#include <stdio.h>
#include <string.h>
#include "commands.h"

int main(void)
{
    FILE* Comfile;
    Comfile =  fopen("TextFiles/CommandFile.txt", "r");

    if (Comfile == NULL)
    {
        printf("\n\nCommand File not open\n\n");
        return 0;
    }

    FILE* ComAssFile;
    ComAssFile = fopen("TextFiles/CommandAssemblyFile.txt", "w");

    char word1[10] = {0};
    int intword2 = 0;
    char chrword2[10] = {0};

    int nstringsscaned = fscanf(Comfile, "%s", word1);
    while (nstringsscaned >= 1)
    {
        if (strcmp(word1, "hlt") == 0) {
            fprintf(ComAssFile, "%d\n", hlt);
        }

        else if (strcmp(word1, "push") == 0)
        {

            if (fscanf(Comfile, "%d", &intword2) == 1)
            {
                fprintf(ComAssFile, "%d ", push);
                fprintf(ComAssFile, "%d\n", intword2);
            }
            else
            {
                fscanf(Comfile, "%s", &chrword2);
                // fprintf(ComAssFile, "push arg is %s\n", chrword2);

                if (strcmp(chrword2, "rax") == 0)
                {

                    fprintf(ComAssFile, "%d ", rpush);
                    fprintf(ComAssFile, "1\n");
                }
                else if (strcmp(chrword2, "rbx") == 0)
                {
                    fprintf(ComAssFile, "%d ", rpush);
                    fprintf(ComAssFile, "2\n");
                }
                else if (strcmp(chrword2, "rcx") == 0)
                {
                    fprintf(ComAssFile, "%d ", rpush);
                    fprintf(ComAssFile, "3\n");
                }
                else if (strcmp(chrword2, "rdx") == 0)
                {
                    fprintf(ComAssFile, "%d ", rpush);
                    fprintf(ComAssFile, "4\n");
                }
                else
                {
                    fprintf(ComAssFile, "%s", "invalid syntax (not register name)\n"); return 0;
                }
            }
        }

        else if (strcmp(word1, "pop") == 0)
        {
            fscanf(Comfile, "%s", &chrword2);
            // fprintf(ComAssFile, "pop arg is %s\n", chrword2);
            if (strcmp(chrword2, "rax") == 0)
            {
                fprintf(ComAssFile, "%d ", rpop);
                fprintf(ComAssFile, "1\n");
            }
            else if (strcmp(chrword2, "rbx") == 0)
            {
                fprintf(ComAssFile, "%d ", rpop);
                fprintf(ComAssFile, "2\n");
            }
            else if (strcmp(chrword2, "rcx") == 0)
            {
                fprintf(ComAssFile, "%d ", rpop);
                fprintf(ComAssFile, "3\n");
            }
            else if (strcmp(chrword2, "rdx") == 0)
            {
                fprintf(ComAssFile, "%d ", rpop);
                fprintf(ComAssFile, "4\n");
            }
            else
            {
                fprintf(ComAssFile, "%s", "invalid syntax (not register name)\n"); return 0;
            }
        }

        else if (strcmp(word1, "add") == 0) {
            fprintf(ComAssFile, "%d\n", add);
        }

        else if (strcmp(word1, "sub") == 0) {
            fprintf(ComAssFile, "%d\n", sub);
        }

        else if (strcmp(word1, "mul") == 0) {
            fprintf(ComAssFile, "%d\n", mul);
        }

        else if (strcmp(word1, "div_") == 0) {
            fprintf(ComAssFile, "%d\n", div_);
        }

        else if (strcmp(word1, "sqrt_") == 0) {
            fprintf(ComAssFile, "%d\n", sqrt_);
        }

        else if (strcmp(word1, "out") == 0) {
            fprintf(ComAssFile, "%d\n", out);
        }

        else if (strcmp(word1, "in") == 0) {
            fprintf(ComAssFile, "%d\n", in);
        }

        else if (strcmp(word1, "rin") == 0) {
            fprintf(ComAssFile, "%d %d\n", in );
        }

        else
        {
            fprintf(ComAssFile, "invalid syntaxis (command not found)");
        }

        nstringsscaned = fscanf(Comfile, "%s", word1);
    }
    return 0;
}
