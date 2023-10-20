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
};

enum RegisterName
{
    rax = 1,
    rbx = 2,
    rcx = 3,
    rdx = 4
};
