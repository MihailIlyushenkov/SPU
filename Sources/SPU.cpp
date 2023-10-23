#include <math.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <stddef.h>

#include "commdefs.h"

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
        default: printf("Fatal error: invalid register name"); break;
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
        printf("jupming to %d cuz %lf > %lf\n", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    StackPush(SPU->vstk, b);
    StackPush(SPU->vstk, a);

    return 0;
}

int JumpAEComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if ((a > b) || iszero(a,b)) {
        printf("jupming to %d cuz %lf >= %lf\n", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    StackPush(SPU->vstk, b);
    StackPush(SPU->vstk, a);

    return 0;
}

int JumpBComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if (a < b) {
        printf("jupming to %d cuz %lf < %lf", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    StackPush(SPU->vstk, b);
    StackPush(SPU->vstk, a);

    return 0;
}

int JumpBEComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if ((a < b) || iszero(a,b)) {
        printf("jupming to %d cuz %lf <= %lf", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    StackPush(SPU->vstk, b);
    StackPush(SPU->vstk, a);
    return 0;
}

int JumpEComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if (iszero(a,b)) {
        printf("jupming to %d cuz %lf == %lf", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    StackPush(SPU->vstk, b);
    StackPush(SPU->vstk, a);
    return 0;
}

int JumpNEComm(SPU_data* SPU, int IPnew)
{
    double a = 0;
    double b = 0;
    StackPop(SPU->vstk, &a);
    StackPop(SPU->vstk, &b);

    if (!iszero(a,b)) {
        printf("jupming to %d cuz %lf != %lf", IPnew, a, b);
        JumpComm(SPU, IPnew);
    }
    else
        SPU->IP += 1;

    StackPush(SPU->vstk, b);
    StackPush(SPU->vstk, a);
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
        StackPush(SPU->cstk, SPU->IP);
        SPU->IP = IPnew - 1;
    }
    return 0;
}

int RetComm(SPU_data* SPU)
{
    elem_t retval = 0;
    StackPop(SPU->cstk, &retval);

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
    SPU->ComBuff = (char*) calloc(FileData.st_size, sizeof(char));

    fread(SPU->ComBuff, sizeof(char), FileData.st_size, ComAssFile);

    return 0;
}

int main()
{
    ErrorType Error = NoError;
    SPU_data SPU = {0};
    SPU.vstk = MakeStack();
    SPU.cstk = MakeStack();

    Error = STACKINIT(SPU.vstk, 5);
    Error = STACKINIT(SPU.cstk, 5);

    if (Error != NoError)
    {
        printf("cant initiate stack.");
        return 0;
    }
    ReadCommands(&SPU, "TextFiles/CommandAssemblyFile.bin");

    int com = 0;

    int arg_int = 0;
    double arg_dbl = 0;

    int STOPFLAG = 0;


    while(!STOPFLAG)
    {
        // DumpSPU(&SPU);
        com = *((char*) (SPU.ComBuff + SPU.IP));
        // printf("com is %d\n", com);

        switch(com){
            case hlt:   STOPFLAG = 1; break;
            case push:  arg_dbl = *((double*) (SPU.ComBuff + SPU.IP + 1));
                            StackPush(SPU.vstk, arg_dbl); SPU.IP += 8; break;

            case add:   AddComm(&SPU); break;
            case sub:   SubComm(&SPU); break;
            case mul:   MulComm(&SPU); break;
            case div_:  DivComm(&SPU); break;
            case sqrt_: SqrtComm(&SPU); break;
            case out:   OutComm(&SPU); break;
            case in:    InComm(&SPU); break;
            case rpush: arg_int = *((int*) (SPU.ComBuff + SPU.IP + 1));
                            RPushComm(&SPU, arg_int);   break;

            case rpop:  arg_int = *((int*) (SPU.ComBuff + SPU.IP + 1));
                            RPopComm(&SPU, arg_int);    break;

            case jump:  arg_int = *((char*) (SPU.ComBuff + SPU.IP + 1));
                            JumpComm(&SPU, arg_int);    break;

            case call:  arg_int = *((char*) (SPU.ComBuff + SPU.IP + 1));
                            CallComm(&SPU, arg_int);    break;

            case ret:   RetComm(&SPU); break;

            case ja:    arg_int = *((char*) (SPU.ComBuff + SPU.IP + 1));
                            JumpAComm(&SPU, arg_int);   break;

            case jae:   arg_int = *((char*) (SPU.ComBuff + SPU.IP + 1));
                            JumpAEComm(&SPU, arg_int);  break;

            case jb:    arg_int = *((char*) (SPU.ComBuff + SPU.IP + 1));
                            JumpBComm(&SPU, arg_int);   break;

            case jbe:   arg_int = *((char*) (SPU.ComBuff + SPU.IP + 1));
                            JumpBEComm(&SPU, arg_int);  break;

            case je:    arg_int = *((char*) (SPU.ComBuff + SPU.IP + 1));
                            JumpEComm(&SPU, arg_int);   break;

            case jne:   arg_int = *((char*) (SPU.ComBuff + SPU.IP + 1));
                            JumpNEComm(&SPU, arg_int);  break;

            default: printf("invalid command code"); return 0;
        }
        SPU.IP += 1;
    }
    return 0;
}

