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
	ihmcolor.obj TextToPrint.obj FigureToPrint.obj \
	TextWindows.obj Toolbar.obj GraphWindows.obj MenuWindows.obj \
	FilesAssociationBox.obj AboutBox.obj Splashscreen.obj MessageBoxNewGraphicMode.obj \
	WndParentGraphProc.obj WndGraphProc.obj WndConsoleParentProc.obj WndConsoleTextProc.obj \
	Transparency.obj

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

