.SUFFIXES: .desc $(SUFFIXES)

SHELL = /bin/sh

SCIDIR=../..
INTERSCI = $(SCIDIR)/bin/intersci-n

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(CC_PICFLAGS) -I$(SCIDIR)/routines
FFLAGS = $(FC_OPTIONS) $(FC_PICFLAGS) -I$(SCIDIR)/routines

all:: message

# Makefile is only used for .desc -> .c 

.desc.c: 
	@echo  generating $*.c 
	@$(INTERSCI) $*  ;

clean	::
	$(RM) *fi.c *fi_builder.sce *fi.tmp *fi.fundef libex* *.lo *.la  *.o *.dia *.so *.sl

distclean :: clean 
	$(RM) loader.sce Makelib
	$(RM) -r .libs

tests : 
	@for i in  01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17  ;\
	do \
		($(SCIDIR)/util/scidem $(SCIDIR) ex$$i.sce ex$$i.dia );\
	done

message:
	@echo "------------------------------------------";
	@echo "At Scilab prompt, enter:";
	@echo "-->exec exXX.sce"; 
	@echo "to execute example XX ";
	@echo "------------------------------------------";




