##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

LOCALIZATION_C_SOURCES = src/c/hashtable_localization.c \
src/c/localization.c \
src/c/setgetlanguage.c \
src/c/InitializeLocalization.c \
src/c/loadsavelanguage.c \
src/c/getentrieshashtable.c \
src/c/loadhashtableslocalization.c \
src/c/TerminateLocalization.c

GATEWAY_C_SOURCES = sci_gateway/c/gw_localization.c \
sci_gateway/c/sci_setlanguage.c \
sci_gateway/c/sci_getlanguage.c \
sci_gateway/c/sci_getlanguagealias.c \
sci_gateway/c/sci_getlanguagestable.c \
sci_gateway/c/sci_addtext.c \
sci_gateway/c/sci_removetext.c \
sci_gateway/c/sci_gettext.c


libscilocalization_la_CFLAGS=  -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/libs/hashtable/ $(XML_FLAGS)

libscilocalization_la_LDFLAGS = $(XML_LIBS)

pkglib_LTLIBRARIES = libscilocalization.la

libscilocalization_la_SOURCES = $(LOCALIZATION_C_SOURCES) $(GATEWAY_C_SOURCES)

libscilocalization_la_LIBADD = 	$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/libs/hashtable/libscihashtable.la

#### Target ######
target=$(datadir)/modules/localization

#### localization : Conf files ####
libscilocalization_la_rootdir = $(target)
libscilocalization_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml


####localization : init scripts #### 
libscilocalization_la_etcdir = $(target)/etc
libscilocalization_la_etc_DATA = etc/localization.quit etc/localization.start

#### localization : gateway declaration #### 
libscilocalization_la_sci_gatewaydir = $(target)/sci_gateway
libscilocalization_la_sci_gateway_DATA = sci_gateway/localization_gateway.xml


####localization : include files ####
libscilocalization_la_includedir=$(sciincludedir)/localization
libscilocalization_la_include_HEADERS = includes/localization.h \
includes/gw_localization.h

include $(top_srcdir)/Makefile.incl.am
