#include <math.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
// #include <cstdlib>
#
#include "StackDef.h"
#include "commands.h"

struct SPU_data
{
    stack* stk = 0;

    double rax = 0;
    double rbx = 0;
    double rcx = 0;
    double rdx = 0;

    int IP = 0;
    char* ComBuff = 0;
    int BuffSize = 0;
};

int AddComm(SPU_data* SPU)
{
    double val1 = 0;
    double val2 = 0;

    StackPop(SPU->stk, &val1);
    StackPop(SPU->stk, &val2);
    StackPush(SPU->stk, val2 + val1);

    return 0;
}

int SubComm(SPU_data* SPU)
{
    double val1 = 0;
    double val2 = 0;

    StackPop(SPU->stk, &val1);
    StackPop(SPU->stk, &val2);
    StackPush(SPU->stk, val2 - val1);

    return 0;
}

int MulComm(SPU_data* SPU)
{
    double val1 = 0;
    double val2 = 0;

    StackPop(SPU->stk, &val1);
    StackPop(SPU->stk, &val2);
    StackPush(SPU->stk, val2 * val1);

    return 0;
}

int DivComm(SPU_data* SPU)
{
    double val1 = 0;
    double val2 = 0;

    StackPop(SPU->stk, &val1);
    StackPop(SPU->stk, &val2);

    StackPush(SPU->stk, val2/val1);

    return 0;
}

int SqrtComm(SPU_data* SPU)
{
    double val1 = 0;
    StackPop(SPU->stk, &val1);

    StackPush(SPU->stk, sqrt(val1));

    return 0;
}

int OutComm(SPU_data* SPU)
{
    double val1 = 0;
    StackPop(SPU->stk, &val1);

    printf("out value is %lf\n", val1);

    return 0;
}

int InComm(SPU_data* SPU)
{
    double val1 = 0;
    printf("enter number: ");
    scanf("%lf", &val1);

    StackPush(SPU->stk, val1);

    return 0;
}

int RPushComm(SPU_data* SPU, int Nreg)
{
    switch(Nreg)
    {
        case 1: StackPop(SPU->stk, &(SPU->rax)); break;
        case 2: StackPop(SPU->stk, &(SPU->rbx)); break;
        case 3: StackPop(SPU->stk, &(SPU->rcx)); break;
        case 4: StackPop(SPU->stk, &(SPU->rdx)); break;
        default: printf("Fatal error: invalid register name"); break;
    }
    return 0;
}

int RPopComm(SPU_data* SPU, int Nreg)
{
    switch(Nreg)
    {
        case 1: StackPush(SPU->stk, SPU->rax); break;
        case 2: StackPush(SPU->stk, SPU->rbx); break;
        case 3: StackPush(SPU->stk, SPU->rcx); break;
        case 4: StackPush(SPU->stk, SPU->rdx); break;
        default: printf("Fatal error: invalid register name"); break;
    }
    return 0;
}

int JumpComm(SPU_data* SPU, int IPnew)
{
    printf("jumping to %d", IPnew);
    if ((IPnew > SPU->BuffSize) || (IPnew < 0))
    {
        printf("invalid jump value %d, must be 0 < IP < %d", IPnew, SPU->BuffSize);
        abort();
    }
    else
    {
        SPU->IP = IPnew;
    }
    return 0;
}

int DumpSPU(SPU_data* SPU)
{
    DUMP(SPU->stk);

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
    SPU.stk = MakeStack();
    Error = STACKINIT(SPU.stk, 5);

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
                            StackPush(SPU.stk, arg_dbl); SPU.IP += 8; break;

            case add:   AddComm(&SPU); break;
            case sub:   SubComm(&SPU); break;
            case mul:   MulComm(&SPU); break;
            case div_:  DivComm(&SPU); break;
            case sqrt_: SqrtComm(&SPU); break;
            case out:   OutComm(&SPU); break;
            case in:    InComm(&SPU); break;
            case rpush: arg_int = *((int*) (SPU.ComBuff + SPU.IP + 1));
                            RPushComm(&SPU, arg_dbl); SPU.IP += 1; break;
            case rpop:  arg_int = *((int*) (SPU.ComBuff + SPU.IP + 1));
                            RPopComm(&SPU, arg_dbl); SPU.IP += 1; break;
            case jump:  arg_int = *((char*) (SPU.ComBuff + SPU.IP + 1));
                            JumpComm(&SPU, arg_int); SPU.IP -= 1; break;
            default: printf("invalid command code"); return 0;
        }
        SPU.IP += 1;
    }
    return 0;
}

