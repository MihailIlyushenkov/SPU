#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commvalues.h"
#include "AssFunck.h"

#define WRITE_int(x) buffwrite_int(buff, &IP, (x))
#define WRITE_dbl(x) buffwrite_dbl(buff, &IP, (x))
#define WRITE_label(x) writelabel(Comfile, ComAssFile, labels, &labelnow, &IP, buff, (x))
#define WRITE_int_inFunc(x) buffwrite_int(buff, IP, (x))


int buffwrite_int(void* buff, int* IP, int value)
{
    *( (int*) ( (int*) buff + *IP) ) = value;
    *IP += 1;
    return 0;
}

int buffwrite_dbl(void* buff, int* IP, double value)
{
    *( (double*) ( (int*) buff + *IP) ) = value;
    *IP += 2;
    return 0;
}

int createlabel(label* label, char* namestr, int labelp)
{
    label->labelp = labelp;
    strcpy(label->name, namestr);
    return 0;
}

int labelvalue(label** labels, label* seeklabel)
{
    int i = 0;
    while ((labels[i] != 0) && (i < MAXLABELSNUM))
    {

        if (strcmp(labels[i]->name, seeklabel->name) == 0)
        {
            printf("returned labelp %d of %s\n",  labels[i]->labelp, labels[i]->name);
            return labels[i]->labelp;
        }
        i++;
    }

    labels[i] = (label*) calloc(sizeof(label), 1);

    strcpy(labels[i]->name, seeklabel->name);
    labels[i]->labelp = seeklabel->labelp;

    printf("found & created label %s to IP %d\n", labels[i]->name, labels[i]->labelp);


    return -1;
}

int showlabel(label* label2show)
{
    printf("labels name is %s, pointer is %d\n", label2show->name, label2show->labelp);
    return 0;
}

int writelabel(FILE* Comfile, FILE* ComAssFile, label** labels, label* labelnow, int* IP, int** buff, CommandType arg)
{
    int labelp = -1;
    char chrwordlabel[LNAMESIZE] = {0};
    fscanf(Comfile, "%s", &chrwordlabel);
    if (chrwordlabel[0] == ':') {
        createlabel(labelnow, (chrwordlabel + 1), *IP);
        labelp = labelvalue(labels, labelnow);

        if (labelp == -1) {
            fprintf(ComAssFile, "invalid label value\n");
        }
        else {
            printf("gonna write %d of label to file\n", labelp);
            WRITE_int_inFunc(labelp);
            fprintf(ComAssFile, "%d ", arg);
            fprintf(ComAssFile, "%d\n", labelp);
        }
    }
    else {
        fprintf(ComAssFile, "invalid syntax\n");
    }
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

    int** buff = (int**) calloc(BUFFSIZE, sizeof(int));
    int IP = 0;

    char chrword1[COMSIZE] = {0};
    int intword = 0;
    double dblword = 0;
    char chrword2[COMSIZE] = {0};

    label labelnow = {0};

    label** labels = (label**) calloc(MAXLABELSNUM, sizeof(char));


    int nstringsscaned = fscanf(Comfile, "%s", chrword1);

    while (nstringsscaned >= 1)
    {
        printf("%s\n", chrword1);

        if (chrword1[0] == ':') {
            printf("i see label %s\n", chrword1);

            createlabel(&labelnow, (chrword1 + 1), IP);
            labelvalue(labels, &labelnow);
            if (labelvalue(labels, &labelnow) == -1)
                 printf("found new label %s on IP %d\n", labelnow.name, labelnow.labelp);
        }
        else if ( (strcmp(chrword1, "jump") == 0) || (strcmp(chrword1, "call") == 0) ||
                        (strcmp(chrword1, "ja") == 0) || (strcmp(chrword1, "jae") == 0) || (strcmp(chrword1, "jb") == 0) ||
                        (strcmp(chrword1, "jbe") == 0) || (strcmp(chrword1, "je") == 0) || (strcmp(chrword1, "jne") == 0)) {

            nstringsscaned = fscanf(Comfile, "%s", chrword1);
            IP += 2;
        }
        else if (strcmp(chrword1, "push") == 0) {
            if (fscanf(Comfile, "%lf", &dblword) == 1) {
                printf("    value in push is %lf\n", dblword);
                IP += 1 + 2;
            }
            else {
                fscanf(Comfile, "%s", &chrword1);
                printf("    value in push is %s\n", chrword1);
                IP += 2;
            }
        }
        else if (strcmp(chrword1, "pop") == 0) {
            nstringsscaned = fscanf(Comfile, "%s", chrword1);
            printf("    value in pop is %s\n", chrword1);
            IP += 2;
        }

        else if ((strcmp(chrword1, "allocmem") == 0) || (strcmp(chrword1, "pushmem") == 0) || (strcmp(chrword1, "popmem") == 0))
        {
            nstringsscaned = fscanf(Comfile, "%s", chrword1);
            nstringsscaned = fscanf(Comfile, "%s", chrword1);
            IP += 2 + 2;
        }
        else if ( (strcmp(chrword1, "freemem") == 0))
        {
            IP += 2;
        }
        else {
            IP += 1;
        }
        nstringsscaned = fscanf(Comfile, "%s", chrword1);
    }
    IP = 0;
    rewind(Comfile);
    int labelp = 0;
    nstringsscaned = fscanf(Comfile, "%s", chrword1);

    printf("LOX");

    while (nstringsscaned >= 1)
    {
        if (chrword1[0] == ':') {
            createlabel(&labelnow, (chrword1 + 1), IP);
            labelvalue(labels, &labelnow);
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
            if (fscanf(Comfile, "%d", &labelp) == 1) {
                WRITE_int(labelp);

                fprintf(ComAssFile, "%d ", jump);
                fprintf(ComAssFile, "%d\n", labelp);
            }
            else {
                WRITE_label(jump);
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

        else if (strcmp(chrword1, "div") == 0) {
            WRITE_int(div_);

            fprintf(ComAssFile, "%d\n", div_);
        }


        else if (strcmp(chrword1, "allocmem") == 0) {
            WRITE_int(allocmem);    //прочитали и записали команду аллокации
            fprintf(ComAssFile, "%d ", allocmem);

            intword = -1;
            int n = fscanf(Comfile, "%d", &intword);
            if ((n == 1) && (0 <= intword) && (intword <= 9)) {

                WRITE_int(intword); //прочитали и записали номер массива для аллокации
                fprintf(ComAssFile, "%d ", intword);

                if (fscanf(Comfile, "%d", &intword) == 1) { //прочитали и записали размер аллоцированной памяти
                    WRITE_dbl( (int) intword);
                    fprintf(ComAssFile, "%d ", intword);
                }
                else {
                    fprintf(ComAssFile, "invalid syntax - invalid array size");
                }
            }
            else {
                fprintf(ComAssFile, "invalid syntax - invalid array number %d", intword);
            }
            fprintf(ComAssFile, "\n");
        }

        else if (strcmp(chrword1, "freemem") == 0) {
            WRITE_int(freemem);    //прочитали и записали команду очистки
            fprintf(ComAssFile, "%d ", freemem);

            intword = -1;
            int n = fscanf(Comfile, "%d", &intword);
            if ((n == 1) && (0 <= intword) && (intword <= 9)) {

                WRITE_int(intword); //прочитали и записали номер массива для очистки
                fprintf(ComAssFile, "%d ", intword);
            }
            else {
                fprintf(ComAssFile, "invalid syntax - invalid array number %d (if -1: cant read)", intword);
            }
            fprintf(ComAssFile, "\n");
        }

        else if (strcmp(chrword1, "pushmem") == 0) {
            WRITE_int(pushmem);    //прочитали и записали команду записи в память
            fprintf(ComAssFile, "%d ", pushmem);

            intword = -1;
            int n = fscanf(Comfile, "%d", &intword);
            if ((n == 1) && (0 <= intword) && (intword <= 9)) {

                WRITE_int(intword); //прочитали и записали номер массива для записи
                fprintf(ComAssFile, "%d ", intword);

                if (fscanf(Comfile, "%d", &intword) == 1) { //прочитали и записали индекс элемента для записи
                    WRITE_dbl( (int) intword);
                    fprintf(ComAssFile, "%d ", intword);
                }
                else {
                    fprintf(ComAssFile, "invalid syntax - invalid element index");
                }
            }
            else {
                fprintf(ComAssFile, "invalid syntax - invalid array number %d", intword);
            }
            fprintf(ComAssFile, "\n");
        }

        else if (strcmp(chrword1, "popmem") == 0) {
            WRITE_int(popmem);    //прочитали и записали команду извлечения из памяти
            fprintf(ComAssFile, "%d ", popmem);

            intword = -1;
            int n = fscanf(Comfile, "%d", &intword);
            if ((n == 1) && (0 <= intword) && (intword <= 9)) {

                WRITE_int(intword); //прочитали и записали номер массива для извлечения
                fprintf(ComAssFile, "%d ", intword);

                if (fscanf(Comfile, "%d", &intword) == 1) { //прочитали и записали индекс элемента для извлечения
                    WRITE_dbl( (int) intword);
                    fprintf(ComAssFile, "%d ", intword);
                }
                else {
                    fprintf(ComAssFile, "invalid syntax - invalid element index");
                }
            }
            else {
                fprintf(ComAssFile, "invalid syntax - invalid array number %d", intword);
            }
            fprintf(ComAssFile, "\n");
        }

        else if (strcmp(chrword1, "ocr") == 0) {
            WRITE_int(ocr);

            fprintf(ComAssFile, "%d\n", ocr);
        }

        else if (strcmp(chrword1, "sin") == 0) {
            WRITE_int(sin_);

            fprintf(ComAssFile, "%d\n", sin_);
        }

        else if (strcmp(chrword1, "cos") == 0) {
            WRITE_int(cos_);

            fprintf(ComAssFile, "%d\n", sqrt_);
        }

        else if (strcmp(chrword1, "sqrt") == 0) {
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

        else if (strcmp(chrword1, "call") == 0) {
            WRITE_int(call);
            if (fscanf(Comfile, "%d", &labelp) == 1) {
                WRITE_int(labelp);

                fprintf(ComAssFile, "%d ", call);
                fprintf(ComAssFile, "%d\n", labelp);
            }
            else {
                WRITE_label(call);
            }
        }

        else if (strcmp(chrword1, "printarr") == 0) {
            WRITE_int(printarr);

            fprintf(ComAssFile, "%d\n", printarr);
        }

        else if (strcmp(chrword1, "ja") == 0) {
            WRITE_int(ja);
            if (fscanf(Comfile, "%d", &labelp) == 1) {
                WRITE_int(labelp);

                fprintf(ComAssFile, "%d ", ja);
                fprintf(ComAssFile, "%d\n", labelp);
            }
            else {
                WRITE_label(ja);
            }
        }

        else if (strcmp(chrword1, "jae") == 0) {
            WRITE_int(jae);
            if (fscanf(Comfile, "%d", &labelp) == 1) {
                WRITE_int(labelp);

                fprintf(ComAssFile, "%d ", jae);
                fprintf(ComAssFile, "%d\n", labelp);
            }
            else {
                WRITE_label(jae);
            }
        }

        else if (strcmp(chrword1, "jb") == 0) {
            WRITE_int(jb);
            if (fscanf(Comfile, "%d", &labelp) == 1) {
                WRITE_int(labelp);

                fprintf(ComAssFile, "%d ", jb);
                fprintf(ComAssFile, "%d\n", labelp);
            }
            else {
                WRITE_label(jb);
            }
        }

        else if (strcmp(chrword1, "jbe") == 0) {
            WRITE_int(jbe);
            if (fscanf(Comfile, "%d", &labelp) == 1) {
                WRITE_int(labelp);

                fprintf(ComAssFile, "%d ", jbe);
                fprintf(ComAssFile, "%d\n", labelp);
            }
            else {
                WRITE_label(jbe);
            }
        }

        else if (strcmp(chrword1, "je") == 0) {
            WRITE_int(je);
            if (fscanf(Comfile, "%d", &labelp) == 1) {
                WRITE_int(labelp);

                fprintf(ComAssFile, "%d ", je);
                fprintf(ComAssFile, "%d\n", labelp);
            }
            else {
                WRITE_label(je);
            }
        }

        else if (strcmp(chrword1, "jne") == 0) {
            WRITE_int(jne);
            if (fscanf(Comfile, "%d", &labelp) == 1) {
                WRITE_int(labelp);

                fprintf(ComAssFile, "%d ", jne);
                fprintf(ComAssFile, "%d\n", labelp);
            }
            else {
                WRITE_label(jne);
            }
        }


        else if (strcmp(chrword1, "ret") == 0) {
            WRITE_int(ret);

            fprintf(ComAssFile, "%d\n", ret);
        }

        else if (strcmp(chrword1, "clear") == 0) {
            WRITE_int(clearCons);

            fprintf(ComAssFile, "%d\n", clearCons);
        }

        else if (strcmp(chrword1, "prchar") == 0) {
            WRITE_int(prchar);

            fprintf(ComAssFile, "%d\n", prchar);
        }

        else if (strcmp(chrword1, "mod") == 0) {
            WRITE_int(modint);
            fprintf(ComAssFile, "%d ", modint);
            if (fscanf(Comfile, "%lf", &dblword) == 1) {
                WRITE_int((int) dblword);
                fprintf(ComAssFile, "%lf\n", dblword);
            }
            else
                fprintf(ComAssFile, "invelid syntaxis - value in mod isnt double");
        }
        else
        {
            printf("invalid huynya %s\n", chrword1);
            fprintf(ComAssFile, "invalid syntaxis (command %s not found)", chrword1);
        }

        nstringsscaned = fscanf(Comfile, "%s", chrword1);
    }
    printf("LOX");

    printf("\n");
    for (int p = 0; (labels[p] != 0) && (p < MAXLABELSNUM); p++)
    {
        printf("%s = %d\n", labels[p]->name, labels[p]->labelp);
    }

    fwrite(buff, sizeof(int), IP, output);

    fclose(Comfile);
    fclose(ComAssFile);
    fclose(output);

    for (int m = 0; m < IP; m++)
    {
        // printf("%p\n", labels[m]);
        if (labels[m] != 0)
            free(labels[m]);
        else
            break;
    }

    return 0;
}
