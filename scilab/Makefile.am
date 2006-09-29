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

sysconf_DATA = $(top_srcdir)/etc/scilab.start \
$(top_srcdir)/etc/scilab.quit

tcl_DATA = $(top_srcdir)/modules/tclsci/tcl/TK_Scilab.tcl \
	$(top_srcdir)/modules/tclsci/tcl/misc.tcl \
	$(top_srcdir)/modules/tclsci/tcl/callbacks.tcl \
	$(top_srcdir)/modules/tclsci/tcl/tkcontrols.tcl \
	$(top_srcdir)/modules/tclsci/tcl/set.tcl \
	$(top_srcdir)/modules/tclsci/tcl/get.tcl \
	$(top_srcdir)/modules/tclsci/tcl/figure.tcl \
	$(top_srcdir)/modules/tclsci/tcl/uicontrol.tcl \
	$(top_srcdir)/modules/tclsci/tcl/uimenu.tcl


##$(top_srcdir)/routines/arpack/libarpack.la 
#\
#	$(top_srcdir)/routines/console/libconsole.la
#$(SCILIBS)
#scilex_DEPENDENCIES		= $(top_srcdir)/routines/console/libconsole.la
#$(top_srcdir)/routines/arpack/libarpack.la 
#$(SCILIBS)
