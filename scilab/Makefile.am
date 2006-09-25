##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

## Process this file with automake to produce Makefile.in (which will be changed by the ./configure)

AUTOMAKE_OPTIONS = 1.9 dist-zip dist-bzip2

SUBDIRS= libs \
	modules

ACLOCAL_AMFLAGS = -I m4/

changelogs = CHANGES

bin_PROGRAMS		= scilex

scilex_SOURCES		= $(top_srcdir)/modules/core/src/fortran/mainsci.f

#CFLAGS = -L/home/sylvestre/dev/scilab4/routines/.libs -lscilab

scilex_DEPENDENCIES = $(top_builddir)/modules/libscilab.la
scilex_LDADD = $(scilex_DEPENDENCIES)

tcl_DATA = modules/tclsci/tcl/TK_Scilab.tcl \
	modules/tclsci/tcl/misc.tcl \
	modules/tclsci/tcl/callbacks.tcl \
	modules/tclsci/tcl/tkcontrols.tcl \
	modules/tclsci/tcl/set.tcl \
	modules/tclsci/tcl/get.tcl \
	modules/tclsci/tcl/figure.tcl \
	modules/tclsci/tcl/uicontrol.tcl \
	modules/tclsci/tcl/uimenu.tcl


##$(top_srcdir)/routines/arpack/libarpack.la 
#\
#	$(top_srcdir)/routines/console/libconsole.la
#$(SCILIBS)
#scilex_DEPENDENCIES		= $(top_srcdir)/routines/console/libconsole.la
#$(top_srcdir)/routines/arpack/libarpack.la 
#$(SCILIBS)
