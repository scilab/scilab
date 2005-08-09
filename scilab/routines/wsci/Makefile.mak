SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..
LIBRARY = $(SCIDIR)/libs/wsci.lib

OBJSC1 = printf.obj FilesAssociations.obj GetOS.obj SciEnv.obj WinConsole.obj \
dde.obj interfdde.obj registry.obj FindScilab.obj clipboard.obj wmcopydata.obj \
wtext.obj wmenu.obj wprinter.obj wgraph.obj winmain.obj \
wgmenu.obj wstatbar.obj gvwprn.obj wmprint.obj wmtex.obj Events.obj

OBJSC2 =readwin.obj wtloop.obj misc.obj \
	command.obj readcons.obj x_zzledt.obj ScilabXloop.obj \
	win_mem_alloc.obj ihmcolor.obj TextToPrint.obj FigureToPrint.obj

OBJSC = $(OBJSC1) $(OBJSC2)

OBJSF = 

include ../../Makefile.incl.mak
CFLAGS = $(CC_OPTIONS) 
FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



#================= resources 

RESOURCES= Rscilab.res

all	:: $(RESOURCES) 

Rscilab.res: Rscilab.rc wresource.h
	$(RC) $(RCVARS) /foRscilab.res Rscilab.rc

GUIFLAGS=-SUBSYSTEM:windows

#=================== lpr ===========================

all:: ../../bin/lpr.exe

../../bin/lpr.exe 	: lpr.obj
	@echo Creation of $*.exe 
	@$(LINKER) -SUBSYSTEM:console -OUT:"../../bin/lpr.exe" lpr.obj \
	$(RESOURCES) $(GUILIBS)

lpr.obj : gvwprn.c 

#================== dependencies
dde.obj: dde.c
interfdde.obj: interfdde.c
registry.obj : registry.c
FindScilab.obj : FindScilab.c
clipboard.obj : clipboard.c
wmcopydata.obj: wmcopydata.c
command.obj: command.c wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h common.h wtext.h
gvwprn.obj: gvwprn.c gvwin.h wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h common.h
ScilabXloop.obj: ScilabXloop.c wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h common.h
lpr.obj: lpr.c gvwprn.c gvwin.h wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h common.h
misc.obj: misc.c wtext.h wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h common.h
readcons.obj: readcons.c readline.c wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h common.h 
readline.obj: readline.c ../machine.h
readwin.obj: readwin.c readline.c wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h common.h wtext.h
win_mem_alloc.obj: win_mem_alloc.c
wgmenu.obj: wgmenu.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h common.h ../os_specific/men_Sutils.h
wgnuplib.obj: wgnuplib.c wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h common.h
wgraph.obj: wgraph.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h common.h ../graphics/scigraphic.h wgraph.h
winmain.obj: winmain.c wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h common.h wtext.h
wmenu.obj: wmenu.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h common.h
wmprint.obj: wmprint.c ../machine.h
wmtex.obj: wmtex.c ../machine.h common.h
wpause.obj: wpause.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h common.h
wprinter.obj: wprinter.c wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h common.h
wstatbar.obj: wstatbar.c
wtext.obj: wtext.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h common.h
wtloop.obj: wtloop.c wtext.h wresource.h wcommon.h \
  ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
  ../graphics/bcg.h ../stack-c.h ../stack-def.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h \
  common.h ../os_specific/Os_specific.h
x_zzledt.obj: x_zzledt.c wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h common.h wtext.h
