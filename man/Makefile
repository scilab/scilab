SHELL = /bin/sh

include ../Path.incl
include ../Makefile.incl

all:: 
	$(SCIDIR)/bin/scilab -nw -f "format.sci"

distclean::
	$(RM)  $(FILES_TO_CLEAN)

cleanall:
	$(RM)  $(FILES_TO_CLEAN) */*.cat
