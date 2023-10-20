#include <stdio.h>
#include <string.h>
#include "commands.h"

// fwrite(buffer, size of element, count, output)

#define WRITE_int(x) buffwrite_int(buff, &IP, (x))
#define WRITE_dbl(x) buffwrite_dbl(buff, &IP, (x))

static const int BUFFSIZE = 1000;

int buffwrite_int(void* buff, int* IP, int value)
{
    *( (char*) ( (char*) buff + *IP) ) = value;
    *IP += 1;
    return 0;
}

int buffwrite_dbl(void* buff, int* IP, double value)
{
    *( (double*) ( (char*) buff + *IP) ) = value;
    *IP += 8;
    return 0;
}

int getlabel(char* charword, int IP)
{
    int LabelValue = 0;
    char* NoLabelCharword = charword + 1;

    if (strcmp(NoLabelCharword, "next") == 0)
        return IP + 1;
    else if (sscanf(NoLabelCharword, "%d", &LabelValue) == 1)
        return LabelValue;
    else
        return -1;
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
    int IP = 0;

    int labelvalue = 0;
    int labeln = -1;

    char chrword1[10] = {0};
    double dblword = 0;
    char chrword2[10] = {0};



    int labels[100] = {-1};

    // int jumpIPs[100] = {-1};
    // int WrongJumpIP = 0;

    int nstringsscaned = fscanf(Comfile, "%s", chrword1);
    while (nstringsscaned >= 1)
    {
        printf("%s\n", chrword1);
        if (chrword1[0] == ':') {
            int labeln = getlabel(chrword1, IP);
            if (labeln != -1) {
                printf("found label %d to IP %d", labeln, IP);
                labels[labeln] = IP;
            }
        }

        else if (strcmp(chrword1, "jump") == 0) {
            nstringsscaned = fscanf(Comfile, "%s", chrword1);
            IP += 2;
        }

        else if (strcmp(chrword1, "push") == 0) {
            if (fscanf(Comfile, "%lf", &dblword) == 1)
                IP += 9;
            else
                IP += 2;
        }
        else if (strcmp(chrword1, "pop") == 0) {
            IP += 2;
        }
        else {
            IP += 1;
        }
        nstringsscaned = fscanf(Comfile, "%s", chrword1);
    }

    IP = 0;
    rewind(Comfile);

    nstringsscaned = fscanf(Comfile, "%s", chrword1);
    while (nstringsscaned >= 1)
    {
        if (chrword1[0] == ':') {
            int labeln = getlabel(chrword1, IP);
            // printf("found label %d to IP %d", labeln, IP);
            if (labeln != -1) {
                labels[labeln] = IP;
            }
        }

        else if (strcmp(chrword1, "hlt") == 0) {
            WRITE_int(hlt);

            fprintf(ComAssFile, "%d\n", hlt);
        }

        else if (strcmp(chrword1, "push") == 0)
        {

            if (fscanf(Comfile, "%lf", &dblword) == 1)
            {
                WRITE_int(push);
                WRITE_dbl(dblword);

                fprintf(ComAssFile, "%d ", push);
                fprintf(ComAssFile, "%lf\n", dblword);
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

        else if (strcmp(chrword1, "pop") == 0)
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

        else if (strcmp(chrword1, "jump") == 0) {
            WRITE_int(jump);
            if (fscanf(Comfile, "%d", &labelvalue) == 1) {
                WRITE_int(labelvalue);

                fprintf(ComAssFile, "%d ", jump);
                fprintf(ComAssFile, "%d\n", labelvalue);
            }
            else {
                fscanf(Comfile, "%s", &chrword2);
                if (chrword2[0] == ':') {
                    labeln = getlabel(chrword2, IP);

                    if (labeln != -1) {

                        if (labels[labeln] == -1) {
                            fprintf(ComAssFile, "unknown label number\n");
                        }
                        else {
                            WRITE_int(labels[labeln]);
                            fprintf(ComAssFile, "%d ", jump);
                            fprintf(ComAssFile, "%d\n", labels[labeln]);
                        }
                    }
                    else
                        fprintf(ComAssFile, "invalid label value");
                }
                else {
                    fprintf(ComAssFile, "invalid syntax\n");
                }
            }
        }

        else if (strcmp(chrword1, "add") == 0) {
            WRITE_int(add);

            fprintf(ComAssFile, "%d\n", add);
        }

        else if (strcmp(chrword1, "sub") == 0) {
            WRITE_int(sub);

            fprintf(ComAssFile, "%d\n", sub);
        }

        else if (strcmp(chrword1, "mul") == 0) {
            WRITE_int(mul);

            fprintf(ComAssFile, "%d\n", mul);
        }

        else if (strcmp(chrword1, "div_") == 0) {
            WRITE_int(div_);

            fprintf(ComAssFile, "%d\n", div_);
        }

        else if (strcmp(chrword1, "sqrt_") == 0) {
            WRITE_int(sqrt_);

            fprintf(ComAssFile, "%d\n", sqrt_);
        }

        else if (strcmp(chrword1, "out") == 0) {
            WRITE_int(out);

            fprintf(ComAssFile, "%d\n", out);
        }

        else if (strcmp(chrword1, "in") == 0) {
            WRITE_int(in);

            fprintf(ComAssFile, "%d\n", in);
        }

        else
        {
            fprintf(ComAssFile, "invalid syntaxis (command not found)");
        }

        nstringsscaned = fscanf(Comfile, "%s", chrword1);
    }

    fwrite(buff, sizeof(char), IP, output);

    return 0;
}
