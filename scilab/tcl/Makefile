SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

include ../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS) $(TK_INC_PATH) $(TCL_INC_PATH) -I. 

all:: browsehelpexe

browsehelpexe: browsehelpexe.c
	$(CC) -o browsehelpexe browsehelpexe.c $(CFLAGS) $(CC_LDFLAGS) $(TCLTK_LIBS) $(XLIBS) -lm

clean::
	@$(RM) -f browsehelpexe

distclean::
	@$(RM) -f browsehelpexe
