SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/time.lib


OBJSC = mattime.obj intgetdate.obj getdate.obj intcalendar.obj inttimer.obj timer.obj intsleep.obj xpause.obj


include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

