#--------------------------------------------------------
# To be customized 
#--------------------------------------------------------

# SCIDIR must be defined here with absolute path 
# SCIDIR=../../..
include ../../../Path.incl 

PROG=myprog 
OBJS=myprog.o my_ode.o

#--------------------------------------------------------
# do not modify below 
#--------------------------------------------------------

all	: .init 
	$(MAKE) $(MAKEFLAGS) -f config/Makefile.all PROG=$(PROG) OBJS="$(OBJS)" SCIDIR=$(SCIDIR)

.init 	: Makefile 
	config/Init $(PROG) $(SCIDIR) 
	touch .init 

clean	: 
	rm -f $(PROG) $(OBJS)

distclean : 
	rm -f $(PROG) $(OBJS) $(PROG).sh .init config/Makefile *.so *.a *.la *.lo
	rm -f config/Makefile.Top 
	rm -f -r .libs 

