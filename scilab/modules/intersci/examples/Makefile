SHELL = /bin/sh

SCIDIR=../..
INTERSCI = $(SCIDIR)/bin/intersci

include ../../Makefile.incl

all:: info 

info:
	@echo "Type \"make /tmp/pgm.o\" to compile pgm program"
	@echo "Type \"make objs\" to compile all test programs"

OBJS= /tmp/ex1c.o /tmp/ex2f.o /tmp/ex4f.o /tmp/ex5f.o  /tmp/ex6c.o \
	/tmp/ex7f.o /tmp/ex8c.o /tmp/ex9f.o /tmp/ex10f.o /tmp/ex11c.o \
	/tmp/ex12c.o /tmp/ex13c.o  /tmp/ex14c.o /tmp/ex15c.o /tmp/ex16c.o

IOBJS= /tmp/ex1fi.o /tmp/ex2fi.o /tmp/ex3fi.o /tmp/ex4fi.o /tmp/ex5fi.o \
	/tmp/ex6fi.o /tmp/ex7fi.o /tmp/ex8fi.o /tmp/ex9fi.o \
	/tmp/ex10fi.o /tmp/ex11fi.o /tmp/ex12fi.o /tmp/ex13fi.o \
	/tmp/ex14fi.o  /tmp/ex15fi.o /tmp/ex16fi.o

objs	: $(OBJS) $(IOBJS)

CFLAGS = $(CC_OPTIONS) $(CC_PICFLAGS) -I$(SCIDIR)/routines
FFLAGS = $(FC_OPTIONS) $(FC_PICFLAGS) -I$(SCIDIR)/routines

/tmp/ex1fi.o: ex1fi.desc
	$(INTERSCI) ex1fi ; ./sedprov ex1fi 
	$(FC) $(FFLAGS) -c ex1fi.f  -o /tmp/ex1fi.o

/tmp/ex1c.o: ex1c.c
	$(CC) $(CFLAGS) -c ex1c.c  -o /tmp/ex1c.o

/tmp/ex2fi.o: ex2fi.desc
	$(INTERSCI) ex2fi ; ./sedprov ex2fi 
	$(FC) $(FFLAGS) -c ex2fi.f  -o /tmp/ex2fi.o

/tmp/ex2f.o: ex2f.f
	$(FC) $(FFLAGS) -c ex2f.f  -o /tmp/ex2f.o

/tmp/ex3fi.o: ex3fi.desc
	$(INTERSCI) ex3fi ; ./sedprov ex3fi 
	$(FC) $(FFLAGS) -c ex3fi.f  -o /tmp/ex3fi.o

/tmp/ex4fi.o: ex4fi.desc
	$(INTERSCI) ex4fi ; ./sedprov ex4fi 
	$(FC) $(FFLAGS) -c ex4fi.f  -o /tmp/ex4fi.o

/tmp/ex4f.o: ex4f.f
	$(FC) $(FFLAGS) -c ex4f.f  -o /tmp/ex4f.o

/tmp/ex5fi.o: ex5fi.desc
	$(INTERSCI) ex5fi ; ./sedprov ex5fi 
	$(FC) $(FFLAGS) -c ex5fi.f  -o /tmp/ex5fi.o

/tmp/ex5f.o: ex5f.f
	$(FC) $(FFLAGS) -c ex5f.f  -o /tmp/ex5f.o

/tmp/ex6fi.o: ex6fi.desc
	$(INTERSCI) ex6fi ; ./sedprov ex6fi 
	$(FC) $(FFLAGS) -c ex6fi.f  -o /tmp/ex6fi.o

/tmp/ex6c.o: ex6c.c
	$(CC) $(CFLAGS) -c ex6c.c  -o /tmp/ex6c.o

/tmp/ex7fi.o: ex7fi.desc
	$(INTERSCI) ex7fi ; ./sedprov ex7fi 
	$(FC) $(FFLAGS) -c ex7fi.f  -o /tmp/ex7fi.o

/tmp/ex7f.o: ex7f.f
	$(FC) $(FFLAGS) -c ex7f.f  -o /tmp/ex7f.o

/tmp/ex8fi.o: ex8fi.desc
	$(INTERSCI) ex8fi ; ./sedprov ex8fi 
	$(FC) $(FFLAGS) -c ex8fi.f  -o /tmp/ex8fi.o

/tmp/ex8c.o: ex8c.c
	$(CC) $(CFLAGS) -c ex8c.c  -o /tmp/ex8c.o

/tmp/ex9fi.o: ex9fi.desc
	$(INTERSCI) ex9fi ; ./sedprov ex9fi 
	$(FC) $(FFLAGS) -c ex9fi.f  -o /tmp/ex9fi.o

/tmp/ex9f.o: ex9f.f
	$(FC) $(FFLAGS) -c ex9f.f  -o /tmp/ex9f.o

/tmp/ex10fi.o: ex10fi.desc
	$(INTERSCI) ex10fi ; ./sedprov ex10fi 
	$(FC) $(FFLAGS) -c ex10fi.f  -o /tmp/ex10fi.o

/tmp/ex10f.o: ex10f.f
	$(FC) $(FFLAGS) -c ex10f.f  -o /tmp/ex10f.o

/tmp/ex11fi.o: ex11fi.desc
	$(INTERSCI) ex11fi ; ./sedprov ex11fi 
	$(FC) $(FFLAGS) -c ex11fi.f  -o /tmp/ex11fi.o

/tmp/ex11c.o: ex11c.c
	$(CC) $(CFLAGS) -c ex11c.c  -o /tmp/ex11c.o

/tmp/ex12fi.o: ex12fi.desc
	$(INTERSCI) ex12fi ; ./sedprov ex12fi 
	$(FC) $(FFLAGS) -c ex12fi.f  -o /tmp/ex12fi.o

/tmp/ex12c.o: ex12c.c
	$(CC) $(CFLAGS) -c ex12c.c  -o /tmp/ex12c.o

/tmp/ex13fi.o: ex13fi.desc
	$(INTERSCI) ex13fi ; ./sedprov ex13fi 
	$(FC) $(FFLAGS) -c ex13fi.f  -o /tmp/ex13fi.o

/tmp/ex13c.o: ex13c.c
	$(CC) $(CFLAGS) -c ex13c.c  -o /tmp/ex13c.o

/tmp/ex14fi.o: ex14fi.desc
	$(INTERSCI) ex14fi ; ./sedprov ex14fi 
	$(FC) $(FFLAGS) -c ex14fi.f  -o /tmp/ex14fi.o

/tmp/ex14c.o: ex14c.c
	$(CC) $(CFLAGS) -c ex14c.c  -o /tmp/ex14c.o


/tmp/ex15fi.o: ex15fi.desc
	$(INTERSCI) ex15fi ; ./sedprov ex15fi 
	$(FC) $(FFLAGS) -c ex15fi.f  -o /tmp/ex15fi.o

/tmp/ex15c.o: ex15c.c
	$(CC) $(CFLAGS) -c ex15c.c  -o /tmp/ex15c.o

/tmp/ex16fi.o: ex16fi.desc
	$(INTERSCI) ex16fi ; ./sedprov ex16fi 
	$(FC) $(FFLAGS) -c ex16fi.f  -o /tmp/ex16fi.o

/tmp/ex16c.o: ex16c.c
	$(CC) $(CFLAGS) -c ex16c.c  -o /tmp/ex16c.o

tests	:  Makefile 
	@$(RM) zallfi.desc
	@cat *.desc > zallfi 
	@mv zallfi zallfi.desc 
	@$(INTERSCI) zallfi > /dev/null ; ./sedprov zallfi
	@$(SCIDIR)/util/scidem $(SCIDIR) zall.sce zallfi.dia	

distclean:: clean

clean	::
	@$(RM)  ex*fi.f ex*fi.sce ex*fi.tmp zallfi zallfi.desc zallfi.sce zallfi.f zallfi.o zallfi.dia
	@$(RM)  $(OBJS) $(IOBJS)
