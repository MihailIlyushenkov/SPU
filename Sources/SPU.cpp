#include <math.h>
#include <sys/stat.h>
#include <stdlib.h>
// #include <cstdlib>
#include <stdio.h>
#include <stddef.h>
#include "commdefs.h"

int invalid = 0;

int iszero(double a, double b)
{
    return (abs(a-b)>0.0001)?0:1;
}

int AddComm(SPU_data* SPU)
{
    double val1 = 0;
    double val2 = 0;

    StackPop(SPU->vstk, &val1);
    StackPop(SPU->vstk, &val2);
    StackPush(SPU->vstk, val2 + val1);

    return 0;
}

int SubComm(SPU_data* SPU)
{
    double val1 = 0;
    double val2 = 0;

    StackPop(SPU->vstk, &val1);
    StackPop(SPU->vstk, &val2);
    StackPush(SPU->vstk, val1 - val2);

    return 0;
}

int MulComm(SPU_data* SPU)
{
    double val1 = 0;
    double val2 = 0;

    StackPop(SPU->vstk, &val1);
    StackPop(SPU->vstk, &val2);
    StackPush(SPU->vstk, val2 * val1);

    return 0;
}

int DivComm(SPU_data* SPU)
{
    double val1 = 0;
    double val2 = 0;

    StackPop(SPU->vstk, &val1);
    StackPop(SPU->vstk, &val2);

    StackPush(SPU->vstk, val2/val1);

    return 0;
}

int SqrtComm(SPU_data* SPU)
{
    double val1 = 0;
    StackPop(SPU->vstk, &val1);

    StackPush(SPU->vstk, sqrt(val1));

    return 0;
}

int SinComm(SPU_data* SPU)
{
    double val1 = 0;
    StackPop(SPU->vstk, &val1);

    StackPush(SPU->vstk, sin(val1));

    return 0;
}

int CosComm(SPU_data* SPU)
{
    double val1 = 0;
    StackPop(SPU->vstk, &val1);

    StackPush(SPU->vstk, cos(val1));

    return 0;
}


int OutComm(SPU_data* SPU)
{
    double val1 = 0;
    StackPop(SPU->vstk, &val1);

    printf("out value is %lf\n", val1);

    return 0;
}

int InComm(SPU_data* SPU)
{
    double val1 = 0;
    printf("enter number: ");
    scanf("%lf", &val1);

    StackPush(SPU->vstk, val1);

    return 0;
}

int RPushComm(SPU_data* SPU, int Nreg)
{
    switch(Nreg)
    {
        case 1: StackPop(SPU->vstk, &(SPU->rax)); break;
        case 2: StackPop(SPU->vstk, &(SPU->rbx)); break;
        case 3: StackPop(SPU->vstk, &(SPU->rcx)); break;
        case 4: StackPop(SPU->vstk, &(SPU->rdx)); break;
        default: printf("Fatal error: invalid register name %d\n", Nreg); break;
    }
    SPU->IP += 1;
    return 0;
}

int RPopComm(SPU_data* SPU, int Nreg)
{
    switch(Nreg)
    {
        case 1: StackPush(SPU->vstk, SPU->rax); break;
        case 2: StackPush(SPU->vstk, SPU->rbx); break;
        case 3: StackPush(SPU->vstk, SPU->rcx); break;
        case 4: StackPush(SPU->vstk, SPU->rdx); break;
        default: printf("Fatal error: invalid register name"); break;
    }
    SPU->IP += 1;
    return 0;

}

int JumpComm(SPU_data* SPU, int IPnew)
{
    // printf("jumping to %d\n", IPnew);
    if ((IPnew > SPU->BuffSize) || (IPnew < 0))
    {
        printf("invalid jump value %d, must be 0 < IP < %d\n", IPnew, SPU->BuffSize);
        abort();
    }
    else
    {
        SPU->IP = IPnew - 1;
    }
    return 0;
}

int JumpAComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if (a > b) {
        // printf("jupming to %d cuz %lf > %lf\n", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    return 0;
}

int JumpAEComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if ((a > b) || iszero(a,b)) {
        // printf("jupming to %d cuz %lf >= %lf\n", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    return 0;
}

int JumpBComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if (a < b) {
        // printf("jupming to %d cuz %lf < %lf\n", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    return 0;
}

int JumpBEComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if ((a < b) || iszero(a,b)) {
        // printf("jupming to %d cuz %lf <= %lf\n", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    return 0;
}

int JumpEComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if (iszero(a,b)) {
        // printf("jupming to %d cuz %lf == %lf\n", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    return 0;
}

int JumpNEComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if (!iszero(a,b)) {
        // printf("jupming to %d cuz %lf != %lf\n", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    return 0;
}

int CallComm(SPU_data* SPU, int IPnew)
{
    // printf("jumping to %d", IPnew);
    if ((IPnew > SPU->BuffSize) || (IPnew < 0))
    {
        printf("invalid jump value %d, must be 0 < IP < %d\n", IPnew, SPU->BuffSize);
        abort();
    }
    else
    {
        // printf("called %d from %d\n", IPnew, SPU->IP);
        double pereh = 0;
        // printf("called %d\n", IPnew);
        StackPush(SPU->cstk, SPU->IP);
        SPU->IP = IPnew - 1;
    }
    return 0;
}

int RetComm(SPU_data* SPU)
{
    elem_t retval = 0;

    StackPop(SPU->cstk, &retval);
    // printf("returned to %d", ((int) retval) + 1);
    SPU->IP = retval/1 + 1;

    // printf("IP after return is %lf or %d\n", retval + 1, SPU->IP);
    return 0;
}

int DumpSPU(SPU_data* SPU)
{
    DUMP(SPU->vstk);

    printf("IP is %d", SPU->IP);
    printf("\nregisters: rax is %lf, rbx if %lf, rcs is %lf, rdx is %lf\n\n", SPU->rax, SPU->rbx, SPU->rcx, SPU->rdx);

    // int cnt = SPU->IP;
    // int flag = 0;

    for (int i = 0; i < SPU->BuffSize; i += 1)
    {
        if ( (i%16 == 0) && (i!= 0))
        {
            printf("\n");
//             if ( (cnt < 0) && (flag == 0) )
//             {
//                 for (int j = 0; j < 16+cnt; printf(".........") && j++);
//                 {;}
//
//                 printf("^\n");
//                 flag = 1;
//             }
        }
        char j = *( SPU->ComBuff + i);
        if (i != SPU->IP) {
            printf("%-8x ", j);
        }
        else {
            printf(">%-8x ", j);
        }
    }

    printf("\n");
    return 0;
}

int PrcharComm(SPU_data* SPU)
{
    double val = 0;
    StackPop(SPU->vstk, &val);
    // printf("puttingchar");
    putchar((char)val);
    return 0;
}

int ClearComm(SPU_data* SPU)
{
    printf("\x1b[d");
    return 0;
}

int AllocmemComm(SPU_data* SPU, int ArrayNumber, int AllocSize)
{
    SPU->IP += 3;
    // printf("allocating array %d with size %d\n", ArrayNumber, AllocSize);
    if ( (SPU->RAM_p)[ArrayNumber] == 0) {
        double* mem = (double*) calloc(AllocSize, sizeof(double));

        // if (ArrayNumber == 1)
        // {
        //     for (int i = 0; i < 1760; i++)
        //     {
        //
        //     }
        // }

        (SPU->RAM_p)[ArrayNumber] = mem;
        return 0;
    }
    else
    {
        printf("IP now is %d", SPU->IP);
        printf("memory allocation error - array %d already exists. Cant allocate it, cuz it must be freed before allocation. Aborting programm...\n", ArrayNumber);
        abort();
        return 0;
    }
}

int FreememComm(SPU_data* SPU, int ArrayNumber)
{
    SPU->IP += 1;
    if ( (SPU->RAM_p)[ArrayNumber] != 0) {
        free((SPU->RAM_p)[ArrayNumber]);
        (SPU->RAM_p)[ArrayNumber] = 0;
        return 0;
    }
    else
    {
        printf("memory free error - array %d already free. Aborting programm...\n", ArrayNumber);
        abort();
        return 0;
    }
}

int PushmemComm(SPU_data* SPU, int ArrayNumber, int ElemIndex)
{
    double StIndx = ElemIndex;
    double Val = -1;

    if (StIndx == -1) {
        StackPop(SPU->vstk, &StIndx);
    }

    int Indx = (int) StIndx;

    SPU->IP += 3;


    StackPop(SPU->vstk, &Val);

    ((SPU->RAM_p)[ArrayNumber]) [Indx] = Val;

    // printf("pushed array %d[%d] with %lf\n", ArrayNumber, Indx, Val);

    return 0;
}

int PopmemComm(SPU_data* SPU, int ArrayNumber, int ElemIndex)
{

    SPU->IP += 3;

    double StIndx = ElemIndex;
    if (StIndx == -1) {
        StackPop(SPU->vstk, &StIndx);
    }
    int Indx = (int) StIndx;

    // if (ArrayNumber == 3)
    //     printf("taknen number from array %d[%d] is %lf\n", ArrayNumber, Indx, ((SPU->RAM_p)[ArrayNumber])[Indx]);

    StackPush(SPU->vstk, ((SPU->RAM_p)[ArrayNumber])[Indx]);
    return 0;
}

int OcrCom(SPU_data* SPU)
{
    double a = 0;
    StackPop(SPU->vstk, &a);

    int b = a;
    a = b;
    StackPush(SPU->vstk, a);

    return 0;
}

int ModComm(SPU_data* SPU, int value)
{
    double stkvalue;
    StackPop(SPU->vstk, &stkvalue);
    // printf("yo poped %lf to divide to %d\n", stkvalue, value);

    int valuetopush = ((int)stkvalue)%value;
    StackPush(SPU->vstk, (double) valuetopush);
    // printf("pushed %lf ok\n", (double) valuetopush);

    SPU->IP += 1;
    return 0;
}

int ReadCommands(SPU_data* SPU, const char * FileName)
{
    FILE* ComAssFile;
    ComAssFile = fopen(FileName, "rb");

    if (ComAssFile == NULL)
    {
        printf("invalid command file name");
        return 0;
    }

    struct stat FileData;
    stat(FileName, &FileData);

    SPU->BuffSize = FileData.st_size;
    SPU->ComBuff = (int*) calloc(FileData.st_size, sizeof(int));

    fread(SPU->ComBuff, sizeof(char), FileData.st_size, ComAssFile);

    return 0;
}

int PrintArrComm(SPU_data* SPU)
{
    printf("size values is %llu  ", SPU->vstk->size);
    printf("size commands is %llu\n", SPU->cstk->size);
    printf("\narray 0:\n");
    printf("i = %.4lf, j = %.4lf, A = %.4lf, B = %.4lf\n", SPU->rax, SPU->rbx, SPU->rcx, SPU->rdx);

    for (int i = 0; i < 16; i++) {
        printf("%d: %lf\n", i, ( (SPU->RAM_p)[0] )[i] );
    }


    // printf("\n\narray 1:\n");
    // for (int i = 0; i < 1760; i++) {
    //     if (i%20 == 0)
    //         printf("\n");
    //     printf("%.1lf ", ( (SPU->RAM_p)[1] )[i] );
    // }

    int k = 0;
    printf("\n");
    printf("46:");
    for (k = 0; 1760>k; k++)
    {

        if (( (SPU->RAM_p)[2] )[k] == 46)
            printf("%d ", k);
    }
    printf("\n");
    printf("44:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 44)
            printf("%d ", k);
    }
    printf("\n");
    printf("45:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 45)
            printf("%d ", k);
    }
    printf("\n");
    printf("126:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 126)
            printf("%d ", k);
    }
    printf("\n");
    printf("58:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 58)
            printf("%d ", k);
    }
    printf("\n");
    printf("59:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 59)
            printf("%d ", k);
    }
    printf("\n");
    printf("61:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 61)
            printf("%d ", k);
    }
    printf("\n");
    printf("33:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 33)
            printf("%d ", k);
    }
    printf("\n");
    printf("42:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 42)
            printf("%d ", k);
    }
    printf("\n");
    printf("35:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 35)
            printf("%d ", k);
    }
    printf("\n");
    printf("36:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 36)
            printf("%d ", k);
    }
    printf("\n");
    printf("64:");
    for (k = 0; 1760>k; k++)
    {
        if (( (SPU->RAM_p)[2] )[k] == 64)
            printf("%d ", k);
    }
    printf("\n");

//     printf("\n\narray2:\n");
//
//     for (int i = 0; i < 1760; i++) {
//         if (i%20 == 0)
//             printf("\n");
//         printf("%.0lf ", ( (SPU->RAM_p)[2] )[i] );
//     }
//
//     for (int i = 0; i < 1760; i++) {
//         if (i%20 == 0)
//             printf("\n");
//         printf("%.0lf ", ( (SPU->RAM_p)[2] )[i] );
//     }

    getchar();
    return 0;
}

#define GETINT(sdv) *((int*) (SPU.ComBuff + SPU.IP + (sdv)))
#define GETDBL(sdv) *((double*) (SPU.ComBuff + SPU.IP + (sdv)))

int main()
{
    printf("\x1b[2J");
    ErrorType Error = NoError;
    SPU_data SPU = {0};
    SPU.vstk = MakeStack();
    SPU.cstk = MakeStack();

    Error = STACKINIT(SPU.vstk, 1800);
    Error = STACKINIT(SPU.cstk, 1800);

    if (Error != NoError)
    {
        printf("cant initiate stack.");
        return 0;
    }
    ReadCommands(&SPU, "TextFiles/CommandAssemblyFile.bin");

    int com = 0;

    int arg_int = 0;
    int arg2_int = 0;
    double arg_dbl = 0;

    int a = 13;
    StackPush(SPU.vstk, a);


    int STOPFLAG = 0;


    while(!STOPFLAG)
    {
        // DumpSPU(&SPU);
        com = *((char*) (SPU.ComBuff + SPU.IP));
        // if (com == push)
        // {
        //     printf("com is %d with %lf\n", com, GETDBL(1));
        // }
        // else
        // printf("com is %d\n", com);

        // if (com == allocmem || com == jump || com == ret)
        // {
        //     int j = getchar();
        //     if (j == 't')
        //     {
        //         DumpSPU(&SPU);
        //     }
        // }

        switch(com){
            case hlt:   STOPFLAG = 1; break;
            case push:  arg_dbl = GETDBL(1);
                            StackPush(SPU.vstk, arg_dbl); SPU.IP += 2; break;

            case add:   AddComm(&SPU); break;
            case sub:   SubComm(&SPU); break;
            case mul:   MulComm(&SPU); break;
            case div_:  DivComm(&SPU); break;
            case sqrt_: SqrtComm(&SPU); break;
            case sin_:  SinComm(&SPU); break;
            case cos_:  CosComm(&SPU); break;

            case out:   OutComm(&SPU); break;
            case in:    InComm(&SPU); break;
            case rpush:     arg_int = GETINT(1);
                            RPushComm(&SPU, arg_int);   break;

            case rpop:      arg_int = GETINT(1);
                            RPopComm(&SPU, arg_int);    break;

            case jump:      arg_int = GETINT(1);
                            JumpComm(&SPU, arg_int);    break;

            case call:      arg_int = GETINT(1);
                            CallComm(&SPU, arg_int);    break;

            case ret:   RetComm(&SPU); break;

            case ja:        arg_int = GETINT(1);
                            JumpAComm(&SPU, arg_int);   break;

            case jae:       arg_int = GETINT(1);
                            JumpAEComm(&SPU, arg_int);  break;

            case jb:        arg_int = GETINT(1);
                            JumpBComm(&SPU, arg_int);   break;

            case jbe:       arg_int = GETINT(1);
                            JumpBEComm(&SPU, arg_int);  break;

            case je:        arg_int = GETINT(1);
                            JumpEComm(&SPU, arg_int);   break;

            case jne:       arg_int = GETINT(1);
                            JumpNEComm(&SPU, arg_int);  break;
            case allocmem:  arg_int = GETINT(1);
                            arg_dbl = GETDBL(2);
                            AllocmemComm(&SPU, arg_int, (int) arg_dbl); break;

            case freemem:   arg_int = GETINT(1);
                            FreememComm(&SPU, arg_int); break;

            case pushmem:   arg_int = GETINT(1);
                            arg_dbl = GETDBL(2);
                            PushmemComm(&SPU, arg_int, (int) arg_dbl); break;

            case popmem:    arg_int = GETINT(1);
                            arg_dbl = GETDBL(2);
                            PopmemComm(&SPU, arg_int, (int) arg_dbl); break;

            case ocr:       OcrCom(&SPU); break;
            case prchar:    PrcharComm(&SPU); break;
            case clearCons: ClearComm(&SPU); break;
            case printarr:  PrintArrComm(&SPU); break;
            case modint:    arg_int = GETINT(1);
                            ModComm(&SPU, arg_int); break;

            default: printf("invalid command code %d", com); DUMP(SPU.vstk); DUMP(SPU.vstk); PrintArrComm(&SPU); return 0;
        }
        SPU.IP += 1;
    }
    return 0;
}

