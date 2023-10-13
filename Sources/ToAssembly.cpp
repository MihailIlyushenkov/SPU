#include <stdio.h>
#include <string.h>
#include "commands.h"

// fwrite(buffer, size of element, count, output)

#define WRITE_int(x) buffwrite_int(buff, &buffptr, (x))
#define WRITE_dbl(x) buffwrite_dbl(buff, &buffptr, (x))

static const int BUFFSIZE = 1000;

int buffwrite_int(void* buff, int* buffptr, int value)
{
    *( (int*) ( (char*) buff + *buffptr) ) = value;
    *buffptr += 4;
    return 0;
}

int buffwrite_dbl(void* buff, int* buffptr, double value)
{
    *( (double*) ( (char*) buff + *buffptr) ) = value;
    *buffptr += 8;
    return 0;
}

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

    FILE* output;
    output = fopen("TextFiles/CommandAssemblyFile.bin", "wb");

    char* buff[BUFFSIZE] = {0};
    int buffptr = 0;

    char word1[10] = {0};
    int intword2 = 0;
    char chrword2[10] = {0};

    int nstringsscaned = fscanf(Comfile, "%s", word1);
    while (nstringsscaned >= 1)
    {
        if (strcmp(word1, "hlt") == 0) {
            WRITE_int(hlt);

            fprintf(ComAssFile, "%d\n", hlt);
        }

        else if (strcmp(word1, "push") == 0)
        {

            if (fscanf(Comfile, "%d", &intword2) == 1)
            {
                WRITE_int(push);
                WRITE_int(intword2);

                fprintf(ComAssFile, "%d ", push);
                fprintf(ComAssFile, "%d\n", intword2);
            }
            else
            {
                fscanf(Comfile, "%s", &chrword2);
                // fprintf(ComAssFile, "push arg is %s\n", chrword2);

                if (strcmp(chrword2, "rax") == 0)
                {
                    WRITE_int(rpush);
                    WRITE_int(rax);

                    fprintf(ComAssFile, "%d ", rpush);
                    fprintf(ComAssFile, "1\n");
                }
                else if (strcmp(chrword2, "rbx") == 0)
                {
                    WRITE_int(rpush);
                    WRITE_int(rbx);

                    fprintf(ComAssFile, "%d ", rpush);
                    fprintf(ComAssFile, "2\n");
                }
                else if (strcmp(chrword2, "rcx") == 0)
                {
                    WRITE_int(rpush);
                    WRITE_int(rcx);

                    fprintf(ComAssFile, "%d ", rpush);
                    fprintf(ComAssFile, "3\n");
                }
                else if (strcmp(chrword2, "rdx") == 0)
                {
                    WRITE_int(rpush);
                    WRITE_int(rdx);

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
                WRITE_int(rpop);
                WRITE_int(rax);

                fprintf(ComAssFile, "%d ", rpop);
                fprintf(ComAssFile, "1\n");
            }
            else if (strcmp(chrword2, "rbx") == 0)
            {
                WRITE_int(rpop);
                WRITE_int(rbx);

                fprintf(ComAssFile, "%d ", rpop);
                fprintf(ComAssFile, "2\n");
            }
            else if (strcmp(chrword2, "rcx") == 0)
            {
                WRITE_int(rpop);
                WRITE_int(rcx);

                fprintf(ComAssFile, "%d ", rpop);
                fprintf(ComAssFile, "3\n");
            }
            else if (strcmp(chrword2, "rdx") == 0)
            {
                WRITE_int(rpop);
                WRITE_int(rdx);

                fprintf(ComAssFile, "%d ", rpop);
                fprintf(ComAssFile, "4\n");
            }
            else
            {
                fprintf(ComAssFile, "%s", "invalid syntax (not register name)\n"); return 0;
            }
        }

        else if (strcmp(word1, "add") == 0) {
            WRITE_int(add);

            fprintf(ComAssFile, "%d\n", add);
        }

        else if (strcmp(word1, "sub") == 0) {
            WRITE_int(sub);

            fprintf(ComAssFile, "%d\n", sub);
        }

        else if (strcmp(word1, "mul") == 0) {
            WRITE_int(mul);

            fprintf(ComAssFile, "%d\n", mul);
        }

        else if (strcmp(word1, "div_") == 0) {
            WRITE_int(div_);

            fprintf(ComAssFile, "%d\n", div_);
        }

        else if (strcmp(word1, "sqrt_") == 0) {
            WRITE_int(sqrt_);

            fprintf(ComAssFile, "%d\n", sqrt_);
        }

        else if (strcmp(word1, "out") == 0) {
            WRITE_int(out);

            fprintf(ComAssFile, "%d\n", out);
        }

        else if (strcmp(word1, "in") == 0) {
            WRITE_int(in);

            fprintf(ComAssFile, "%d\n", in);
        }

        else
        {
            fprintf(ComAssFile, "invalid syntaxis (command not found)");
        }

        nstringsscaned = fscanf(Comfile, "%s", word1);
    }

    fwrite(buff, sizeof(char), buffptr, output);

    return 0;
}
