SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/wsci.a

OBJSC1 =wtext.o wgnuplib.o wmenu.o wprinter.o wpause.o wgraph.o winmain.o \
	wmhelp.o wgmenu.o wstatbar.o gvwprn.o wmprint.o wmtex.o

OBJSC2 =readwin.o wtloop.o misc.o \
	command.o readcons.o x_zzledt.o jpc_Xloop.o sh.o \
	strings.o files.o tclWinError.o 

OBJSC = $(OBJSC1) $(OBJSC2)

OBJSF = 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) -I../ -Wall

FFLAGS = $(FC_OPTIONS)

include ../Make.lib



#================== dependencies 

wgraph.o : ../graphics/periWin-bcg.h
wmhelp.o : ../sun/h_help.h ../graphics/periWin-bcg.h ../machine.h
$(OBJSC) : wgnuplib.h
readcons.o	: readline.c wtextc.h 
readgcwin.o	: readline.c 
readwin.o 	: readline.c

#================= resources 

RESOURCES= Rscilab.o

all	:: $(RESOURCES) 

Rscilab.o	: Rscilab.res 
	$(CVTRES) Rscilab.res Rscilab.o

Rscilab.res: Rscilab.rc wresource.h
	$(RC) $(RCVARS) /foRscilab.res Rscilab.rc

#===================test ========================

test	: $(RESOURCES) wtest.o ../../libs/wsci.a
	$(CC) $(CFLAGS) $(LINKEROPT) -o ../../bin/test.exe wtest.o \
	$(RESOURCES) ../../libs/wsci.a ../../libs/graphics.a \
	../../libs/wsci.a ../../libs/menusX.a \
	../../libs/sun.a ../../libs/system.a ../../libs/xdr.a -Wl,-subsystem,console $(WINLIBS)

wtest.o : wtloop.c 

#=================== lpr ===========================

all:: ../../bin/lpr.exe

../../bin/lpr.exe : lpr.o 
	$(CC) $(CFLAGS) -DGVW_ALONE $(LINKEROPT)  -o $@ \
	lpr.o  $(WIN32LIBS)

lpr.o : gvwprn.c 

#=================== runscilab========================

RUNOBJSC = runscilab.o
RUNRESOURCES= Rrunscilab.o

RUNLINKEROPT=-Wl,-subsystem,windows,-S

all	:: ../../bin/runscilab.exe

../../bin/runscilab.exe 	: $(RUNOBJSC) $(RUNRESOURCES)
	$(CC) $(CFLAGS) $(RUNLINKEROPT) -o ../../bin/runscilab.exe $(RUNOBJSC) \
	$(RUNRESOURCES) $(WIN32LIBS)

all	:: $(RUNRESOURCES) 

Rrunscilab.o	: Rrunscilab.res 
	$(CVTRES) Rrunscilab.res Rrunscilab.o

Rrunscilab.res: Rrunscilab.rc 
	$(RC) $(RCVARS) /foRrunscilab.res Rrunscilab.rc

clean	::
	$(RM) $(RUNOBJSC) $(RUNRESOURCES) Rsunscilab.res

distclean::
	$(RM) $(RUNOBJSC) $(RUNRESOURCES) Rsunscilab.res
