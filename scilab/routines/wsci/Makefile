SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/wsci.a

OBJSC1 = printf.o FilesAssociations.o GetOS.o SciEnv.o WinConsole.o dde.o interfdde.o registry.o FindScilab.o \
clipboard.o wmcopydata.o wtext.o wgnuplib.o wmenu.o wprinter.o wpause.o wgraph.o \
wgmenu.o wstatbar.o gvwprn.o wmprint.o wmtex.o winmain.o Events.o
	

OBJSC2 =readwin.o wtloop.o misc.o \
	command.o readcons.o x_zzledt.o jpc_Xloop.o sh.o \
	strings.o files.o tclWinError.o 

OBJSC = $(OBJSC1) $(OBJSC2)
OBJSF = 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH) 
FFLAGS = $(FC_OPTIONS)

include ../Make.lib

#================== dependencies 
clipboard.o : clipboard.c
wmcopydata.o : wmcopydata.c
wgraph.o : ../graphics/periWin-bcg.h
$(OBJSC) : wgnuplib.h
readcons.o	: readline.c wtextc.h 
readgcwin.o	: readline.c 
readwin.o 	: readline.c

#================= mainapp 

all	:: ../../libs/scimain.o

../../libs/scimain.o	: 
	$(CC) $(CFLAGS) -o ../../libs/scimain.o -c scimain.c 

distclean::
	@$(RM) ../../libs/scimain.o 

#================= resources 

RESOURCES= Rscilab.o

all	:: $(RESOURCES) 

Rscilab.o	: Rscilab.rc wresource.h 
	$(RC) Rscilab.rc $(RCVARS) -o Rscilab.o 

#===================test ========================

test	: $(RESOURCES) wtest.o ../../libs/wsci.a
	$(CC) $(CFLAGS) $(LINKEROPT) -o ../../bin/test.exe wtest.o \
	$(RESOURCES) ../../libs/wsci.a ../../libs/graphics.a \
	../../libs/wsci.a ../../libs/menusX.a \
	../../libs/sun.a ../../libs/system.a ../../libs/xdr.a -Wl,-subsystem,console -lg2c $(WINLIBS)

wtest.o : wtloop.c 

#=================== lpr ===========================

all:: ../../bin/lpr.exe

../../bin/lpr.exe : lpr.o 
	$(CC) $(CFLAGS) -DGVW_ALONE $(LINKEROPT)  -o $@ $^  $(WIN32LIBS)

lpr.o : gvwprn.c 

distclean::
	@$(RM) ../../bin/lpr.exe 

#=================== runscilab========================

all	:: ../../bin/runscilab.exe

../../bin/runscilab.exe: runscilab.o Rrunscilab.o
	$(CC) $(CFLAGS) -o $@ $^ $(WIN32LIBS)

Rrunscilab.o: Rrunscilab.rc wresource.h 
	$(RC) Rrunscilab.rc $(RCVARS) -o Rrunscilab.o 

distclean::
	@$(RM) ../../bin/runscilab.exe 

#=================== general ========================

clean	::
	@$(RM) *.o 

distclean :: clean 
