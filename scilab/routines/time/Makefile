SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/time.a


OBJSC = gw_time.o \
        sci_getdate.o getdate.o \
        sci_calendar.o \
        sci_timer.o timer.o \
        sci_sleep.o sci_xpause.o \
        sci_realtime.o realtime.o


include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

