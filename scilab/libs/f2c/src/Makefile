# Makefile for f2c, a Fortran 77 to C converter
SCIDIR=../../..

include $(SCIDIR)/Makefile.incl

CC=gcc
CFLAGS = $(CC_OPTIONS)
SHELL = /bin/sh

OBJECTSd = main.o init.o gram.o lex.o proc.o equiv.o data.o format.o \
	  expr.o exec.o intr.o io.o misc.o error.o mem.o names.o \
	  output.o p1output.o pread.o put.o putpcc.o vax.o formatdata.o \
	  parse_args.o niceprintf.o cds.o sysdep.o version.o
OBJECTS = $(OBJECTSd) 
#OBJECTS = $(OBJECTSd) malloc.o

all::  $(SCIDIR)/bin/f2c

$(SCIDIR)/bin/f2c: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(SCIDIR)/bin/f2c

#	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(SCIDIR)/bin/f2c -lmmalloc

$(OBJECTSd): defs.h ftypes.h defines.h machdefs.h sysdep.h

tokdefs.h: tokens
	grep -n . <tokens | sed "s/\([^:]*\):\(.*\)/#define \2 \1/" >tokdefs.h

cds.o: sysdep.h
exec.o: p1defs.h names.h
expr.o: output.h niceprintf.h names.h
format.o: p1defs.h format.h output.h niceprintf.h names.h iob.h
formatdata.o: format.h output.h niceprintf.h names.h
gram.o: p1defs.h
init.o: output.h niceprintf.h iob.h
intr.o: names.h
io.o: names.h iob.h
lex.o : tokdefs.h p1defs.h
main.o: parse.h usignal.h
mem.o: iob.h
names.o: iob.h names.h output.h niceprintf.h
niceprintf.o: defs.h names.h output.h niceprintf.h
output.o: output.h niceprintf.h names.h
p1output.o: p1defs.h output.h niceprintf.h names.h
parse_args.o: parse.h
proc.o: tokdefs.h names.h niceprintf.h output.h p1defs.h
put.o: names.h pccdefs.h p1defs.h
putpcc.o: names.h
vax.o: defs.h output.h pccdefs.h
output.h: niceprintf.h

put.o putpcc.o: pccdefs.h

f2c.t: f2c.1t
	troff -man f2c.1t >f2c.t

#f2c.1: f2c.1t
#	nroff -man f2c.1t | col -b | uniq >f2c.1

clean::
	rm -f  *.o f2c tokdefs.h f2c.t *.obj 

distclean :: clean 

b = Notice README cds.c data.c defines.h defs.h equiv.c error.c \
	exec.c expr.c f2c.1 f2c.1t f2c.h format.c format.h formatdata.c \
	ftypes.h gram.dcl gram.exec gram.expr gram.head gram.io \
	init.c intr.c io.c iob.h lex.c machdefs.h main.c \
	malloc.c mem.c memset.c misc.c names.c names.h niceprintf.c \
	niceprintf.h output.c output.h p1defs.h p1output.c \
	parse.h parse_args.c pccdefs.h pread.c proc.c put.c putpcc.c \
	sysdep.c sysdep.h tokens usignal.h vax.c version.c xsum.c

xsum: xsum.c
	$(CC) $(CFLAGS) -o xsum xsum.c

#Check validity of transmitted source...
xsum.out: xsum $b
	./xsum $b >xsum1.out
	cmp xsum0.out xsum1.out && mv xsum1.out xsum.out

#On non-Unix systems that end lines with carriage-return/newline pairs,
#use "make xsumr.out" rather than "make xsum.out".  The -r flag ignores
#carriage-return characters.
xsumr.out: xsum $b
	./xsum -r $b >xsum1.out
	cmp xsum0.out xsum1.out && mv xsum1.out xsumr.out
