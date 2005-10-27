# Makefile for f2c, a Fortran 77 to C converter
SCIDIR=../../..
SCIDIR1=..\..\..

include ../../../Makefile.incl.mak

OBJECTSd = main.obj init.obj gram.obj lex.obj proc.obj equiv.obj \
	data.obj format.obj \
	expr.obj exec.obj intr.obj io.obj misc.obj error.obj mem.obj \
	names.obj output.obj p1output.obj pread.obj put.obj putpcc.obj \
	vax.obj formatdata.obj \
	parse_args.obj niceprintf.obj cds.obj sysdep.obj version.obj

OBJECTS = $(OBJECTSd) 

#OBJECTS = $(OBJECTSd) malloc.obj

CFLAGS=$(CC_OPTIONS) -DMSDOS

all::  $(SCIDIR)/bin/f2c.exe

$(SCIDIR)/bin/f2c.exe	: $(OBJECTS)
	@echo ------- Building $@  --------
	@$(LINKER) $(LINKER_FLAGS) $(GUIFLAGS) -OUT:"$(SCIDIR)/bin/f2c.exe" $(OBJECTS)

# @$(LINKER) $(LINKER_FLAGS) $(GUIFLAGS) -OUT:"$(SCIDIR)/bin/f2c.exe" $(OBJECTS)
# @$(LINKER) $(LINKER_FLAGS) $(GUIFLAGS) -OUT:"$(SCIDIR)/bin/f2c.exe" $(OBJECTS) $(GUILIBS) 

$(OBJECTSd): defs.h ftypes.h defines.h machdefs.h sysdep.h

cds.obj: sysdep.h
exec.obj: p1defs.h names.h
expr.obj: output.h niceprintf.h names.h
format.obj: p1defs.h format.h output.h niceprintf.h names.h iob.h
formatdata.obj: format.h output.h niceprintf.h names.h
gram.obj: p1defs.h
init.obj: output.h niceprintf.h iob.h
intr.obj: names.h
io.obj: names.h iob.h
lex.obj : tokdefs.h p1defs.h
main.obj: parse.h usignal.h
mem.obj: iob.h
names.obj: iob.h names.h output.h niceprintf.h
niceprintf.obj: defs.h names.h output.h niceprintf.h
output.obj: output.h niceprintf.h names.h
p1output.obj: p1defs.h output.h niceprintf.h names.h
parse_args.obj: parse.h
proc.obj: tokdefs.h names.h niceprintf.h output.h p1defs.h
put.obj: names.h pccdefs.h p1defs.h
putpcc.obj: names.h
vax.obj: defs.h output.h pccdefs.h
output.h: niceprintf.h
put.obj putpcc.obj: pccdefs.h

clean::
	-del *.obj *.o 

distclean:: clean 
	-del $(SCIDIR1)\bin\f2c.exe
