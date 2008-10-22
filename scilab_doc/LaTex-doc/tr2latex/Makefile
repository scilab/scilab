SHELL = /bin/sh

include ../../../Makefile.incl
 
# define NO_SGTTY because SGTTY not useful for our use of tr2latex

CFLAGS = $(CC_OPTIONS) -DNO_SGTTY

# Use makefile.msc if you are compiling with MS-DOS
# add -Dtops20 to CFLAGS if you're running it under tops20
# and add -DANSI if you're using ANSI C

OBJS = tr2tex.o tr.o subs.o

PROGRAMS = tr2tex

all:: $(PROGRAMS)

$(PROGRAMS): $(OBJS)
	$(RM) $@
	$(CC) -o $@ $(OBJS) $(CC_LDFLAGS)

clean::
	$(RM) $(OBJS)
distclean::
	$(RM) $(OBJS) $(PROGRAMS)

tr2tex.o: setups.h 
tr.o: setups.h
subs.o: setups.h
subs.o: simil.h greek.h flip.h forbid.h maths.h macros.h
