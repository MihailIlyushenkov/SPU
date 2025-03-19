const int BUFFSIZE = 2000;
const int LNAMESIZE = 30;
const int COMSIZE = 20;
const int MAXLABELSNUM = 100;

struct label
{
    char name[LNAMESIZE] = {0};
    int labelp = -1;
};

int buffwrite_int(void* buff, int* IP, int value);
int buffwrite_dbl(void* buff, int* IP, double value);
int createlabel(label* label, char* namestr, int labelp);
int labelvalue(label** labels, label* seeklabel);
int showlabel(label* label2show);
int writelabel(FILE* Comfile, FILE* ComAssFile, label** labels, label* labelnow, int* IP, char** buff, CommandType arg);
