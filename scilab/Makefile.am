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

scilex_DEPENDENCIES = $(top_builddir)/modules/libscilab.la
scilex_LDADD = $(scilex_DEPENDENCIES)

sysconf_DATA = $(top_srcdir)/etc/scilab.start \
$(top_srcdir)/etc/scilab.quit

all-local:
#	.libs/scilex -nb -l eng -nouserstartup -nwni -e "xmltohtml()";exit;
#	.libs/scilex -nb -l fr -nouserstartup -nwni -e "xmltohtml()";exit;


check-local:
#	@..\..\..\bin\scilex -nb -l fr -e check_help();exit;
#	@..\..\..\bin\scilex -nb -l eng -e check_help();exit;

distclean-local::	
clean-local:
#	@..\..\..\bin\scilex -nb -l eng -nouserstartup -nwni -e clean_help();exit;
#	@..\..\..\bin\scilex -nb -l fr -nouserstartup -nwni -e clean_help();exit;
