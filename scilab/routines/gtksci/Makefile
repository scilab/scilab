# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh

SCIDIR=../..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/gtksci.a

OBJSC =	click.o command.o  io.o \
	menus.o x_VTPrsTbl.o x_main.o \
	zzledt.o about.o queue.o helpbrowser.o 

OBJSF  = 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH)
FFLAGS = $(FC_OPTIONS)

include ../Make.lib

# need gnome includes 

helpbrowser.o : helpbrowser.c 
	$(CC) $(CC_OPTIONS) `gnome-config --cflags gnome` -c helpbrowser.c 


version.h: version.g $(SCIDIR)/Version.incl
	@$(RM) version.h
	@sed -e "s+SCILAB_VERSION+$(SCIVERSION)+" version.g > version.h;
	@chmod g+w version.h
	@echo version.h created

helpbrowser:  helpbrowser.c queue.o
	$(CC) -DALONE $(CC_OPTIONS) `gnome-config --cflags gnome` \
		-o helpbrowser helpbrowser.c queue.o \
		`gnome-config --cflags gnome` $(XLIBS)

OBJTEST = x_test_loop.o

# test 

test: $(OBJTEST) $(OBJSC) ../sun/zzledt.o ../sun/flags.o ../sun/h_help_data.o ../sun/men_Sutils.o ../sun/tmpdir.o ../sun/timer.o
	cd ../graphics; make ;cd ../xsci
	$(RM) $@
	$(CC) -g -o $@ $(OBJTEST) ../sun/zzledt.o ../sun/flags.o ../sun/h_help_data.o ../sun/tmpdir.o \
	../sun/men_Sutils.o ../sun/inffic.o ../sun/timer.o \
	$(OBJSC) $(SCIDIR)/libs/graphics.a $(SCIDIR)/libs/menusX.a \
	$(SCIDIR)/ $(CC_LDFLAGS) $(XLIBS) $(TERMCAPLIB) \
	$(TCLTK_LIBS) -lm

clean::
	$(RM) $(OBJTEST)
distclean::
	$(RM) $(OBJTEST) test


# generated with gcc -MMD 

click.o: click.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
  ../sun/men_Sutils.h ../menusX/men_scilab.h All-extern.h  \
  ../graphics/periX11-bcg.h
command.o: command.c ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h All-extern.h ../sun/Sun.h
help.o: help.c version.h ../sun/h_help.h ../machine.h ../graphics/Math.h \
  ../graphics/Graphics.h
io.o: io.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
  ../sun/Sun.h All-extern.h
jpc_xwidgets.o: jpc_xwidgets.c ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h
menus.o: menus.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
  ../graphics/periGtk-bcg.h  All-extern.h \
   ../sun/Sun.h 
x_main.o: x_main.c version.h ../machine.h ../graphics/Math.h \
  ../graphics/Graphics.h x_ptyxP.h x_ptyx.h x_error.h x_menu.h \
  All-extern.h

x_test_loop.o: x_test_loop.c ../machine.h ../menusX/men_scilab.h \
  ../graphics/Math.h ../graphics/Graphics.h ../sun/men_Sutils.h \
   All-extern.h
x_VTPrsTbl.o: x_VTPrsTbl.c x_VTparse.h
