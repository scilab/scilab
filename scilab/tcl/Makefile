SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

include ../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS) $(TK_INC_PATH) $(TCL_INC_PATH) -I. 

all::
	$(CC)  $(CFLAGS) $(CFLAGS) $(TCLTK_LIBS) $(XLIBS) browsehelpexe.c -o browsehelpexe

clean::
	-rm browsehelpexe
	-rm *.o

