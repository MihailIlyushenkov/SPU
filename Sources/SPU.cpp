#include <math.h>
#include "StackDef.h"
#include "textsave.h"
#include "commands.h"

struct SPU_data
{
    stack* stk = 0;

    double rax = 0;
    double rbx = 0;
    double rcx = 0;
    double rdx = 0;

    int ComPointer = 0;
    int ComArray[50] = {0};
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

int DumpSPU(SPU_data* SPU, int iteration, Text ComText)
{
    DUMP(SPU->stk);
    printf("registers: rax is %lf, rbx if %lf, rcs is %lf, rdx is %lf\n\n", SPU->rax, SPU->rbx, SPU->rcx, SPU->rdx);
    return 0;
}

int main()
{

    ErrorType Error = NoError;
    SPU_data SPU = {0};
    SPU.stk = MakeStack();
    Error = STACKINIT(SPU.stk, 5);

    Text MyText = {};
    ReadTextFromFile(&MyText, "TextFiles/CommandAssemblyFile.txt");

    int com = 0;
    double arg = 0;

    for(size_t i = 0; (i < MyText.NumberOfLines); i++)
    {
        // DumpSPU(&SPU, i, MyText);

        sscanf((MyText.TextPivots)[i], "%d", &com);
        switch(com){
            case hlt: return 0;
            case push:  sscanf((MyText.TextPivots)[i], "%d %lf", &com, &arg);
                            StackPush(SPU.stk, arg); break;

            case add: AddComm(&SPU); break;
            case sub: SubComm(&SPU); break;
            case mul: MulComm(&SPU); break;
            case div_: DivComm(&SPU); break;
            case sqrt_: SqrtComm(&SPU); break;
            case out: OutComm(&SPU); break;
            case in: InComm(&SPU); break;
            case rpush: sscanf((MyText.TextPivots)[i], "%d %lf", &com, &arg); RPushComm(&SPU, arg); break;
            case rpop:  sscanf((MyText.TextPivots)[i], "%d %lf", &com, &arg); RPopComm(&SPU, arg); break;
            default: printf("invalid command code"); return 0;
        }
    }
    return 0;
}

