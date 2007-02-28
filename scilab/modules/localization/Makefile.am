##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

LOCALIZATION_C_SOURCES = src/c/hashtable_localization.c \
src/c/localization.c \
src/c/setgetlanguage.c \
src/c/InitializeLocalization.c \
src/c/loadsavelanguage.c

GATEWAY_C_SOURCES = sci_gateway/gw_localization.c \
sci_gateway/sci_setlanguage.c \
sci_gateway/sci_getlanguage.c \
sci_gateway/sci_getlanguagealias.c \
sci_gateway/sci_getlanguagestable.c

libscilocalization_la_CFLAGS=  -Iincludes/ -Isrc/ -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/libs/hashtable/ $(XML_FLAGS)

libscilocalization_la_LDFLAGS = $(XML_LIBS)

pkglib_LTLIBRARIES = libscilocalization.la

libscilocalization_la_SOURCES = $(LOCALIZATION_C_SOURCES) $(GATEWAY_C_SOURCES)

libscilocalization_la_LIBADD = 	$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/libs/hashtable/libscihashtable.la

#### Target ######
target=$(datadir)/localization

#### localization : Conf files ####
libscilocalization_la_rootdir = $(target)
libscilocalization_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


####localization : init scripts #### 
libscilocalization_la_etcdir = $(target)/etc
libscilocalization_la_etc_DATA = etc/localization.quit etc/localization.start


####localization : include files ####
libscilocalization_la_includedir=$(sciincludedir)/localization
libscilocalization_la_include_HEADERS = includes/localization.h \
includes/gw_localization.h

include $(top_srcdir)/Makefile.incl.am
