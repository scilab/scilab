SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/time.a


OBJSC = mattime.o intgetdate.o getdate.o intcalendar.o inttimer.o timer.o intsleep.o intxpause.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

