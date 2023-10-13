#include <math.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "StackDef.h"
#include "commands.h"

struct SPU_data
{
    stack* stk = 0;

    double rax = 0;
    double rbx = 0;
    double rcx = 0;
    double rdx = 0;

    int ComPointer = 0;
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

int DumpSPU(SPU_data* SPU)
{
    DUMP(SPU->stk);
    printf("registers: rax is %lf, rbx if %lf, rcs is %lf, rdx is %lf\n\n", SPU->rax, SPU->rbx, SPU->rcx, SPU->rdx);



    for (int i = 0; i < SPU->BuffSize; i += 4)
    {
        if (i%64 == 0)
            printf("\n");

        short j = *((short*) (SPU->ComBuff + i));
        printf("%x ", j);
    }

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
    double arg = 0;

    int STOPFLAG = 0;


    while(!STOPFLAG)
    {
        // DumpSPU(&SPU);
        com = *((int*) (SPU.ComBuff + SPU.ComPointer));
        // printf("com is %d\n", com);

        switch(com){
            case hlt: STOPFLAG = 1; break;
            case push:  arg = SPU.ComBuff[SPU.ComPointer + 4];
                            StackPush(SPU.stk, arg); SPU.ComPointer += 4; break;

            case add: AddComm(&SPU); break;
            case sub: SubComm(&SPU); break;
            case mul: MulComm(&SPU); break;
            case div_: DivComm(&SPU); break;
            case sqrt_: SqrtComm(&SPU); break;
            case out: OutComm(&SPU); break;
            case in: InComm(&SPU); break;
            case rpush: arg = SPU.ComBuff[SPU.ComPointer + 4]; RPushComm(&SPU, arg); SPU.ComPointer += 4; break;

            case rpop:  arg = SPU.ComBuff[SPU.ComPointer + 4]; RPopComm(&SPU, arg); SPU.ComPointer += 4; break;
            default: printf("invalid command code"); return 0;
        }

        SPU.ComPointer += 4;
    }
    return 0;
}

