
# the list of files to produce the library 
OBJS = tdll.o
LD=ld
CC=gcc
CFLAGS=-O
LDFLAGS = --noinhibit-exec
DLLTOOL= dlltool --as=as

LDFLAGS-libtdll = --image-base=0x11000000 

all: libtdll.dll libtdll.stubs.a 

libtdll.a: $(OBJS)
	rm -f libtdll.a
	ar cr libtdll.a $(OBJS)
	ranlib libtdll.a

include Makefile.DLLs

clean	:
	rm -f libtdll.* lib_name libs_name *.o 
	rm -f libs_path

distclean : clean 
