enum CommandType
{
    hlt = -1,
    push = 0,
    add = 1,
    sub = 2,
    mul = 3,
    div_ = 4,
    sqrt_ = 5,
    out = 6,
    in = 7,
    rpush = 33,
    rpop = 43,
    jump = 28,

    ja = 50,
    jae = 51,
    jb = 52,
    jbe = 53,
    je = 54,
    jne = 55,

    call = 65,
    ret = 66,

    slay = 99
};

enum RegisterName
{
    rax = 1,
    rbx = 2,
    rcx = 3,
    rdx = 4
};
