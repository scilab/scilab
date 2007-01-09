##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


GATEWAY_C_SOURCES = sci_gateway/sci_mcisendstring.c \
sci_gateway/sci_toolbar.c \
sci_gateway/sci_printbox.c \
sci_gateway/sci_consoledos.c \
sci_gateway/intpathname.c \
sci_gateway/sci_about.c \
sci_gateway/sci_winqueryreg.c \
sci_gateway/gw_wintools.c \
sci_gateway/sci_winopen.c \
sci_gateway/sci_setlanguagemenu.c \
sci_gateway/sci_fileassociationbox.c \
sci_gateway/sci_dos.c \
sci_gateway/intihmcolor.c \
sci_gateway/sci_dde.c \
sci_gateway/intOEMconvert.c \
sci_gateway/sci_toprint.c \
sci_gateway/sci_clipboard.c

libsciwintools_la_CFLAGS= -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libsciwintools.la

libsciwintools_la_SOURCES = $(GATEWAY_C_SOURCES)

libsciwintools_la_LIBADD = \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la

libsciwintools_la_PKGCONFIG = wintools.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/wintools


#### wintools : Conf files ####
libsciwintools_la_rootdir = $(target)
libsciwintools_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### wintools : Scilab macros ###
libsciwintools_la_macrodir = $(target)/macros
libsciwintools_la_macro_DATA22 =  macros/buildmacros.sce 


#### wintools : init scripts #### 
libsciwintools_la_etcdir = $(target)/etc
libsciwintools_la_etc_DATA = etc/wintools.quit etc/wintools.start 


#### wintools : gateway declaration #### 
libsciwintools_la_sci_gatewaydir = $(target)/sci_gateway
libsciwintools_la_sci_gateway_DATA = sci_gateway/wintools

#### wintools : include files ####
libsciwintools_la_includedir=$(includedir)/wintools
libsciwintools_la_include_HEADERS = includes/sci_dde.h \
includes/intOEMconvert.h \
includes/sci_clipboard.h \
includes/sci_toprint.h \
includes/sci_toolbar.h \
includes/sci_printbox.h \
includes/interfdde.h \
includes/TextToPrint.h \
includes/sci_consoledos.h \
includes/sci_mcisendstring.h \
includes/sci_about.h \
includes/intpathname.h \
includes/gw_wintools.h \
includes/sci_winopen.h \
includes/sci_winqueryreg.h \
includes/sci_setlanguagemenu.h \
includes/sci_fileassociationbox.h \
includes/sci_dos.h \
includes/intihmcolor.h

include $(top_srcdir)/Makefile.incl.am
