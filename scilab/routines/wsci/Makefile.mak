SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..
LIBRARY = $(SCIDIR)/libs/wsci.lib

OBJSC1 = FindScilab.obj clipboard.obj wmcopydata.obj wtext.obj wgnuplib.obj wmenu.obj wprinter.obj wpause.obj wgraph.obj winmain.obj \
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

#================== dependencies
FindScilab.obj : FindScilab.c
clipboard.obj : clipboard.c
wmcopydata.obj: wmcopydata.c
abs_main.obj: abs_main.c
abs_putenv.obj: abs_putenv.c
command.obj: command.c wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h ../graphics/Entities.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h wadd.h common.h plot.h wtext.h
files.obj: files.c dstrings.h
gvwprn.obj: gvwprn.c gvwin.h wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../graphics/Entities.h ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h wadd.h common.h
jpc_Xloop.obj: jpc_Xloop.c wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../graphics/Entities.h ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h wadd.h common.h
lpr.obj: lpr.c gvwprn.c gvwin.h wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../graphics/Entities.h ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h wadd.h common.h
misc.obj: misc.c plot.h wtext.h wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../graphics/Entities.h ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h wadd.h common.h
rdl-nw-test.obj: rdl-nw-test.c
readcons.obj: readcons.c readline.c wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../graphics/Entities.h ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h wadd.h common.h wtextc.h
readline.obj: readline.c ../machine.h
readwin.obj: readwin.c readline.c wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h ../graphics/Entities.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h wadd.h common.h wtext.h
runscilab.obj: runscilab.c
scimain.obj: scimain.c
sh.obj: sh.c common.h ../machine.h
strings.obj: strings.c dstrings.h
tclWinError.obj: tclWinError.c
wgmenu.obj: wgmenu.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h ../graphics/Entities.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h wadd.h common.h ../sun/men_Sutils.h
wgnuplib.obj: wgnuplib.c wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../graphics/Entities.h ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h wadd.h common.h
wgraph.obj: wgraph.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h ../graphics/Entities.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h wadd.h common.h ../graphics/scigraphic.h wgraph.h
winmain.obj: winmain.c wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h ../graphics/Entities.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h wadd.h common.h plot.h wtext.h
wmenu.obj: wmenu.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h ../graphics/Entities.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h wadd.h common.h
wmprint.obj: wmprint.c ../machine.h
wmtex.obj: wmtex.c ../machine.h common.h
wpause.obj: wpause.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h ../graphics/Entities.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h wadd.h common.h
wprinter.obj: wprinter.c wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../graphics/Entities.h ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h wadd.h common.h
wstatbar.obj: wstatbar.c
wtest.obj: wtest.c wtloop.c plot.h wtext.h wresource.h wcommon.h \
  ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
  ../graphics/bcg.h ../graphics/Entities.h ../stack-c.h ../stack-def.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h wadd.h \
  common.h ../sun/Sun.h
wtext.obj: wtext.c wresource.h wcommon.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../graphics/bcg.h ../graphics/Entities.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h wadd.h common.h
wtloop.obj: wtloop.c plot.h wtext.h wresource.h wcommon.h \
  ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
  ../graphics/bcg.h ../graphics/Entities.h ../stack-c.h ../stack-def.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h wadd.h \
  common.h ../sun/Sun.h
x_zzledt.obj: x_zzledt.c wresource.h wcommon.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../graphics/bcg.h \
  ../graphics/Entities.h ../stack-c.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h wadd.h common.h plot.h wtext.h
