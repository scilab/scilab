#--------------------------------------------------------
# To be customized 
#--------------------------------------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..


JAVAINC= -I/usr/lib/SunJava2-1.4.1/include -I/usr/lib/SunJava2-1.4.1/include/linux

#--------------------------------------------------------
LIBRARY= $(SCIDIR)/bin/libjavasci.so 
JAVAC=javac
SOURCESJ=*.java
OBJSC= javasci_Matrix.o fix.o

OBJSF = 

include ../../Makefile.incl
include ../../Makefile.OBJ

CFLAGS = $(CC_OPTIONS) $(JAVAINC)

OBJS = $(OBJSC)

all:: class all1

all1:: $(LIBRARY)
# options compilation a modifier sous hp & solaris
$(LIBRARY): $(OBJS)
	@echo Creation of $(LIBRARY)
	@$(RM) $@;
	@echo "linking";
	@$(FC) -Wl,-shared -g -o $@ $(OBJS) $(DEFAULTS) $(LIBR) $(TCLTK_LIBS) $(XLIBS) $(TERMCAPLIB);

class::	
	$(JAVAC) -d $(SCIDIR)/bin $(SOURCESJ)
clean::
	$(RM) *.o

cleanC :
	$(RM) $(OBJSC)
	$(RM) $(SCIDIR)/bin/javasci/*.class

distclean::
	$(RM) *.o $(LIBRARY)
