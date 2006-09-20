## Process this file with automake to produce Makefile.in (which will be changed by the ./configure)

SUBDIRS= libs \
	modules

ACLOCAL_AMFLAGS = -I m4/

changelogs = CHANGES

bin_PROGRAMS		= scilex

scilex_SOURCES		= \
	modules/core/src/fortran/mainsci.f

#CFLAGS = -L/home/sylvestre/dev/scilab4/routines/.libs -lscilab
#scilex_LDADD = 	$(top_builddir)/routines/libscilab.la 

scilex_DEPENDENCIES = $(top_builddir)/modules/libscilab.la
scilex_LDADD = $(scilex_DEPENDENCIES)

tcl_DATA = tcl/TK_Scilab.tcl \
	tcl/misc.tcl \
	tcl/callbacks.tcl \
	tcl/tkcontrols.tcl \
	tcl/set.tcl \
	tcl/get.tcl \
	tcl/figure.tcl \
	tcl/uicontrol.tcl \
	tcl/uimenu.tcl


##$(top_srcdir)/routines/arpack/libarpack.la 
#\
#	$(top_srcdir)/routines/console/libconsole.la 
#$(SCILIBS)
#scilex_DEPENDENCIES		= $(top_srcdir)/routines/console/libconsole.la 
#$(top_srcdir)/routines/arpack/libarpack.la 
#$(SCILIBS)
