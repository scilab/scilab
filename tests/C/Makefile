#--------------------------------------------------------
# To be customized 
#--------------------------------------------------------

# SCIDIR must be defined here with absolute path
# SCIDIR=../..
include ../../Path.incl

PROG=Test1
OBJS=Test1.o

#--------------------------------------------------------
# do not modify below
#--------------------------------------------------------

all	: .init
	$(MAKE) $(MAKEFLAGS) -f config/Makefile.all PROG=$(PROG) OBJS="$(OBJS)" SCIDIR=$(SCIDIR)
	cp $(PROG) $(SCIDIR)/bin/$(PROG)

.init 	: Makefile
	config/Init $(PROG) $(SCIDIR)
	touch .init

clean	:
	rm -f $(PROG) $(OBJS)
	rm -f $(SCIDIR)/bin/$(PROG)

distclean :
	rm -f $(PROG) $(OBJS) $(PROG).sh .init config/Makefile *.so *.a *.la *.lo
	rm -f $(SCIDIR)/bin/$(PROG)
	rm -f config/Makefile.Top
	rm -f -r .libs

