CC = g++
CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

all: NormalStart

NormalStart: ToAssembly ArrayMk3.o  SPU.o StackBody.o
	Objects\ToAssembly.exe && $(CC) Objects\ArrayMk3.o Objects\SPU.o Objects\StackBody.o -o SPU.exe

test: ToAssembly Disassembly
	Objects\ToAssembly.exe && Objects\Disassembly.exe

ToAssembly: ToAssembly.o
	$(CC) $(CFLAGS) Objects\ToAssembly.o -o Objects\ToAssembly.exe

Disassembly: Disassembly.o
	$(CC) $(CFLAGS) Objects\Disassembly.o -o Objects\Disassembly.exe

Disassembly.o: Sources\Disassembly.cpp
	$(CC) $(CFLAGS) -c Sources\Disassembly.cpp -o Objects\Disassembly.o

ToAssembly.o: Sources\ToAssembly.cpp
	$(CC) $(CFLAGS) -c Sources\ToAssembly.cpp -o Objects\ToAssembly.o

ArrayMk3.o: Sources\ArrayMk3.cpp
	$(CC) $(CFLAGS) -c Sources\ArrayMk3.cpp -o Objects\ArrayMk3.o

SPU.o: Sources\SPU.cpp
	$(CC) $(CFLAGS) -c Sources\SPU.cpp -o Objects\SPU.o

StackBody.o: Sources\StackBody.cpp
	$(CC) $(CFLAGS) -c Sources\StackBody.cpp -o Objects\StackBody.o

clean:
	del Objects\*.o Objects\*.exe TextFiles\CommandAssemblyFile.txt TextFiles\DisassembledCommandFile.txt
