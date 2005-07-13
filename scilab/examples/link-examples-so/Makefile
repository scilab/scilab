SHELL = /bin/sh
SCIDIR=../..

include $(SCIDIR)/Makefile.incl

CFLAGS = $(CC_OPTIONS) $(CC_PICFLAGS) -I$(SCIDIR)/routines
FFLAGS = $(FC_OPTIONS) $(FC_PICFLAGS) -I$(SCIDIR)/routines

all:: message

job	: 
	make -f Makelib

TESTS= 1c 2c 3c 4c 5c 6c 7c 8c 9c 10c 11c 12c 13c 14c \
	1f 2f 3f 4f 5f 6f 7f 8f 9f 10f 11f 12f 13f 14f 

tests : 
	@for i in  $(TESTS) ;\
	do \
		$(SCIDIR)/util/scidem $(SCIDIR) ext$$i.sce ext$$i.dia ;\
	done

message:
	@echo "------------------------------------------";
	@echo "At Scilab prompt, enter:";
	@echo "-->exec exXX.sce"; 
	@echo "to execute example XX "
	@echo "------------------------------------------";

clean	::
	@echo Cleaning 
	@$(RM) *fi.c *fi_builder.sce *fi.tmp *fi.fundef libex* *.lo *.la  *.o *.dia *.so loader.sce Makelib *.sl
	@$(RM) -f -r .libs 

distclean:: clean 



