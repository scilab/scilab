SHELL = /bin/sh
SCIDIR=..

include $(SCIDIR)/Version.incl
include $(SCIDIR)/Makefile.incl

CFLAGS = $(CC_OPTIONS) -I../routines/

OBJ = intersci.o
PROGRAM = $(SCIDIR)/bin/intersci

OBJ1 = intersci-n.o getrhs.o crerhs.o variables.o read.o fornames.o out.o outext.o declare.o opt.o check.o
PROGRAM1 = $(SCIDIR)/bin/intersci-n

all:: $(PROGRAM) $(PROGRAM1)

$(PROGRAM): $(OBJ)
	$(RM) $(PROGRAM)
	$(CC) -o $(PROGRAM) $(OBJ) $(CC_LDFLAGS)

$(PROGRAM1): $(OBJ1)
	$(RM) $(PROGRAM1)
	$(CC) -o $(PROGRAM1) $(OBJ1) $(CC_LDFLAGS)

clean::
	$(RM) $(OBJ) $(OBJ1)
distclean:: clean 
	$(RM) $(PROGRAM1) $(PROGRAM)  *.fundef ex*.f

$(OBJ): intersci.h
$(OBJ1) : intersci-n.h 




