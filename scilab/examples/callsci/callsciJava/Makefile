#--------------------------------------------------------
# To be customized 
#--------------------------------------------------------

SCIDIR=/usr/local/scilab-X11

JAVAID=/usr/java/j2sdk1.4.2_03/include
JAVAINC=-I$(JAVAID) -I$(JAVAID)/linux

#--------------------------------------------------------
# do not modify below 
#--------------------------------------------------------

PROG= libjavasci.so 
OBJS= javasci_Matrix.o fix.o

all	:: .init  
	make -f config/Makefile.all  FC="g77 -Wl,-shared" PROG=$(PROG) OBJS="$(OBJS)" SCIDIR=$(SCIDIR) \
		JAVAINC="$(JAVAINC)"
	cd javasci; make all SCIDIR=$(SCIDIR)
	cd example; make all SCIDIR=$(SCIDIR)

.init 	: Makefile 
	config/Init $(PROG) $(SCIDIR) 
	touch .init 

clean	::
	@rm  -f $(PROG) $(OBJS)

distclean ::
	@rm -f $(PROG) $(OBJS) $(PROG).sh .init config/Makefile 
	@rm -f config/Makefile.Top
	@find classes -name '*.class' -exec \rm -f {} \;

 
