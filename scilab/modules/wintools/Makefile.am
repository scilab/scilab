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

libwintools_la_includedir=$(includedir)/wintools

libwintools_la_include_HEADERS = includes/intOEMconvert.h \
includes/sci_dde.h \
includes/sci_mcisendstring.h \
includes/sci_toprint.h \
includes/sci_clipboard.h \
includes/sci_toolbar.h \
includes/sci_printbox.h \
includes/interfdde.h \
includes/sci_consoledos.h \
includes/intpathname.h \
includes/sci_about.h \
includes/sci_winqueryreg.h \
includes/gw_wintools.h \
includes/sci_winopen.h \
includes/sci_setlanguagemenu.h \
includes/sci_fileassociationbox.h \
includes/sci_dos.h \
includes/intihmcolor.h \
includes/TextToPrint.h

libwintools_la_CFLAGS= -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libwintools.la

libwintools_la_SOURCES = $(GATEWAY_C_SOURCES)

libwintools_la_LIBADD = \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libwintools_la_PKGCONFIG = wintools.pc
