##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


GATEWAY_C_SOURCES = sci_gateway/c/sci_mcisendstring.c \
sci_gateway/c/sci_toolbar.c \
sci_gateway/c/sci_printbox.c \
sci_gateway/c/sci_consoledos.c \
sci_gateway/c/intpathname.c \
sci_gateway/c/sci_about.c \
sci_gateway/c/sci_winqueryreg.c \
sci_gateway/c/gw_wintools.c \
sci_gateway/c/sci_winopen.c \
sci_gateway/c/sci_setlanguagemenu.c \
sci_gateway/c/sci_fileassociationbox.c \
sci_gateway/c/sci_dos.c \
sci_gateway/c/intihmcolor.c \
sci_gateway/c/sci_dde.c \
sci_gateway/c/intOEMconvert.c \
sci_gateway/c/sci_toprint.c \
sci_gateway/c/sci_clipboard.c \
sci_gateway/c/sci_findfileassociation.c

libsciwintools_la_CFLAGS= -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

pkglib_LTLIBRARIES = libsciwintools.la

libsciwintools_la_SOURCES = $(GATEWAY_C_SOURCES)

libsciwintools_la_LIBADD = \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la




#### Target ######
modulename=wintools


#### wintools : Conf files ####
libsciwintools_la_rootdir = $(mydatadir)
libsciwintools_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

#### wintools : init scripts #### 
libsciwintools_la_etcdir = $(mydatadir)/etc
libsciwintools_la_etc_DATA = etc/wintools.quit etc/wintools.start 


#### wintools : gateway declaration #### 
libsciwintools_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciwintools_la_sci_gateway_DATA = sci_gateway/wintools_gateway.xml

#### wintools : include files ####
libsciwintools_la_includedir=$(pkgincludedir)/wintools
libsciwintools_la_include_HEADERS = includes/gw_wintools.h \
includes/interfdde.h \
includes/TextToPrint.h

include $(top_srcdir)/Makefile.incl.am
