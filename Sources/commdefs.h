#include "StackDef.h"
#include "commvalues.h"

struct SPU_data
{
    stack* vstk = 0;
    stack* cstk = 0;

    double rax = 0;
    double rbx = 0;
    double rcx = 0;
    double rdx = 0;

    int IP = 0;
    char* ComBuff = 0;
    int BuffSize = 0;
};

int iszero(double a, double b);
int AddComm(SPU_data* SPU);
int SubComm(SPU_data* SPU);
int MulComm(SPU_data* SPU);
int DivComm(SPU_data* SPU);
int SqrtComm(SPU_data* SPU);
int OutComm(SPU_data* SPU);
int InComm(SPU_data* SPU);
int RPushComm(SPU_data* SPU, int Nreg);
int RPopComm(SPU_data* SPU, int Nreg);
int JumpComm(SPU_data* SPU, int IPnew);
int JumpAComm(SPU_data* SPU, int IPnew);
int JumpAEComm(SPU_data* SPU, int IPnew);
int JumpBComm(SPU_data* SPU, int IPnew);
int JumpBEComm(SPU_data* SPU, int IPnew);
int JumpEComm(SPU_data* SPU, int IPnew);
int JumpNEComm(SPU_data* SPU, int IPnew);
int CallComm(SPU_data* SPU, int IPnew);
int RetComm(SPU_data* SPU);

int ReadCommands(SPU_data* SPU, const char * FileName);
int DumpSPU(SPU_data* SPU);
