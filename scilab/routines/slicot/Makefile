SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/slicot.a

OBJSC =

OBJSF = ib01py.o mb01sd.o mb03od.o sb02ms.o sb02rd.o ib01bd.o \
	ma02ad.o mb02pd.o mb04kd.o sb02mt.o sb02ru.o \
	ib01pd.o ma02ed.o mb02qy.o sb02nd.o sb02sd.o \
	ib01px.o mb01ru.o mb02ud.o sb02mr.o sb02qd.o \
	mb01rx.o mb01ry.o mb01ud.o mb01vd.o mb03ud.o \
	mb04od.o mb04oy.o sb03mx.o sb03my.o sb03qx.o \
	sb03qy.o sb03sx.o sb03sy.o sb03mv.o sb03mw.o \
	sb04px.o select.o \
	ib01ad.o ib01cd.o ib01md.f \
	ib01nd.o ib01od.o ib01qd.o ib01rd.o tb01wd.o \
	ib01md.o ib01oy.o mb01td.o mb04id.o mb04iy.o \
	ib01my.o ma02fd.o \

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile
