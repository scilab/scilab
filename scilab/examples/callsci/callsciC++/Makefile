#--------------------------------------------------------
# To be customized 
#--------------------------------------------------------

SCIDIR=../../..

PROG=myprog 
OBJS=myprog.o ccmatrix1.o

#--------------------------------------------------------
# do not modify below 
#--------------------------------------------------------

all	: .init 
	make -f config/Makefile.all PROG=$(PROG) OBJS="$(OBJS)" SCIDIR=$(SCIDIR)

.init 	: Makefile 
	config/Init $(PROG) $(SCIDIR) 
	touch .init 

clean	: 
	@rm -f $(PROG) $(OBJS)

distclean : 
	@rm -f $(PROG) $(OBJS) callsci.sh .init 
	@rm -f libodeex.* Makelib my_ode.o my_ode.lo 

