#--------------------------------------------------------
# To be customized 
#--------------------------------------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

JAVAID=/usr/java/j2sdk1.4.2_03/include
JAVAINC=-I$(JAVAID) -I$(JAVAID)/linux

#--------------------------------------------------------
PROG= $(SCIDIR)/libs/javasci.so 
OBJS= javasci_Matrix.o fix.o

all	:: .init  
	make -f config/Makefile.all  FC="g77 -Wl,-shared" PROG=$(PROG) OBJS="$(OBJS)" SCIDIR=$(SCIDIR) \
		JAVAINC="$(JAVAINC)"
	cd classes; make all SCIDIR=../$(SCIDIR)


.init 	: Makefile 
	config/Init $(PROG) $(SCIDIR) 
	touch .init 

clean	::
	@rm  -f $(PROG) $(OBJS)

distclean ::
	@rm -f $(PROG) $(OBJS) $(PROG).sh .init config/Makefile 
	@rm -f config/Makefile.Top
	@find classes -name '*.class' -exec \rm -f {} \;

 
