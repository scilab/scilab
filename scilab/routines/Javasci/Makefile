#--------------------------------------------------------
# To be customized 
#--------------------------------------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

JAVAID=/usr/java/j2sdk1.4.2_06/include
JAVAINC=-I$(JAVAID) -I$(JAVAID)/linux

#--------------------------------------------------------
LIBRARY= $(SCIDIR)/bin/libjavasci.so 


OBJSC= javasci_Matrix.o fix.o

OBJSF = 

include ../../Makefile.incl
include ../../Makefile.OBJ

CFLAGS = $(CC_OPTIONS) $(JAVAINC)

OBJS = $(OBJSC)

all:: $(LIBRARY)

$(LIBRARY): $(OBJS)
	@echo Creation of $(LIBRARY)
	$(RM) $@;
	echo "linking";
	$(FC) -o $@ $(OBJS) $(DEFAULTS) $(LIBR) $(TCLTK_LIBS) $(XLIBS) $(TERMCAPLIB);

clean::
	$(RM) *.o

cleanC :
	$(RM) $(OBJSC)

distclean::
	$(RM) *.o $(LIBRARY)
