#--------------------------------------------------------
# To be customized 
#--------------------------------------------------------

SCIDIR=../../../

PROG=callsci
OBJS=callsci.o

#--------------------------------------------------------
# do not modify below 
#--------------------------------------------------------

all	: .init 
	make -f config/Makefile.all PROG=$(PROG) OBJS="$(OBJS)" SCIDIR=$(SCIDIR)

.init 	: Makefile 
	config/Init $(PROG) $(SCIDIR) 
	touch .init 

clean	: 
	rm -f $(PROG) $(OBJS)

distclean : 
	rm -f $(PROG) $(OBJS) $(PROG).sh .init config/Makefile 
	rm -f config/Makefile.Top

 
