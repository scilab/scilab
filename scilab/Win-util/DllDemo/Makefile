# Dll demo file for cygwin 

OBJS = tdll.o
LD=ld
CC=gcc -mno-cygwin
CFLAGS=-O  
LDFLAGS = --noinhibit-exec
DLLTOOL= dlltool --as=as

all: libtdll.dll libtdll.stubs.a test1 test2 

libtdll.a: $(OBJS)
	rm -f libtdll.a
	ar cr libtdll.a $(OBJS)
	ranlib libtdll.a

include Makefile.DLLs

# test Version with link at startup 

test1	: test1.c libtdll.a 
	$(CC) $(CFLAGS) $(LINKEROPT) -o test1.exe test1.c libtdll.stubs.a 

# Version with LoadLibrary 
# 

test2	: test2.c libtdll.a 
	$(CC) $(CFLAGS) $(LINKEROPT)  -o test2.exe test2.c 

clean	:
	rm -f libtdll.a libtdll.dll libtdll.stubs.a lib_name libs_name
	rm -f test1.exe test2.exe *.o libtdll.def libtdll.exp libtdll.base
	rm -f libs_path tdll.base *~ *.*~

distclean: clean





