SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..
LIBRARY = $(SCIDIR)/libs/wsci.lib

OBJSC1 =wtext.obj wgnuplib.obj wmenu.obj wprinter.obj wpause.obj wgraph.obj winmain.obj \
	wgmenu.obj wstatbar.obj gvwprn.obj wmprint.obj wmtex.obj

OBJSC2 =readwin.obj wtloop.obj misc.obj \
	command.obj readcons.obj x_zzledt.obj jpc_Xloop.obj sh.obj \
	strings.obj files.obj tclWinError.obj 

OBJSC = $(OBJSC1) $(OBJSC2)

OBJSF = 

include ../../Makefile.incl.mak
CFLAGS = $(CC_OPTIONS) 
FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

#================== dependencies 

wgraph.obj : ../graphics/periWin-bcg.h
$(OBJSC) : wgnuplib.h

#================= resources 

RESOURCES= Rscilab.res

all	:: $(RESOURCES) 

Rscilab.res: Rscilab.rc wresource.h
	$(RC) $(RCVARS) /foRscilab.res Rscilab.rc

GUIFLAGS=-SUBSYSTEM:windows

#===================test ========================
!IF "$(DTK)" == "-DWITH_TK"
GTK_LIBS= ../../libs/tksci.lib ../../libs/intersci.lib ../../libs/libf2c.lib $(XLIBS)
!ELSE 
GTK_LIBS= $(XLIBS)
!ENDIF

test	: $(RESOURCES) wtest.obj ../../libs/wsci.lib 
	@echo Creation of ../../bin/test-vc.exe
	@$(LINKER) $(LINKER_FLAGS) $(GUIFLAGS) \
	 -OUT:"../../bin/test-vc.exe" wtest.obj \
	$(RESOURCES) ../../libs/wsci.lib ../../libs/graphics.lib \
	../../libs/wsci.lib ../../libs/menusX.lib \
	../../libs/sun.lib ../../libs/system.lib ../../libs/xdr.lib \
	$(GTK_LIBS) 

wtest.obj : wtloop.c 

#=================== lpr ===========================

all:: ../../bin/lpr.exe

../../bin/lpr.exe 	: lpr.obj
	@echo Creation of $*.exe 
	@$(LINKER) -SUBSYSTEM:console -OUT:"../../bin/lpr.exe" lpr.obj \
	$(RESOURCES) $(GUILIBS)

lpr.obj : gvwprn.c 

#=================== runscilab========================

RUNRESOURCES= Rrunscilab.res

all:: ../../bin/runscilab.exe

../../bin/runscilab.exe: runscilab.c  $(RUNRESOURCES) 
	@echo Creation of $*.exe 
	-del runscilab.obj 
	@$(CC) $(CFLAGS) -DTEST runscilab.c 	
	@$(LINKER) -SUBSYSTEM:windows -OUT:"../../bin/runscilab.exe" \
	runscilab.obj $(RESOURCES) $(GUILIBS) 

# resources 

all	:: $(RUNRESOURCES) 

Rrunscilab.res: Rrunscilab.rc 
	$(RC) $(RCVARS) /foRrunscilab.res Rrunscilab.rc

readcons.obj	: readline.c wtextc.h 
readgcwin.obj	: readline.c 
readwin.obj 	: readline.c




