##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007
##########

LOCALIZATION_C_SOURCES = \
src/c/InitializeLocalization.c \
src/c/setgetlanguage.c \
src/c/loadsavelanguage.c
#src/c/TerminateLocalization.c

if GUI
#LOCALIZATION_C_SOURCES += src/c/org_scilab_modules_localization_QueryString.c
endif


GATEWAY_C_SOURCES = sci_gateway/c/gw_localization.c \
sci_gateway/c/sci_gettext.c \
sci_gateway/c/sci_setlanguage.c \
sci_gateway/c/sci_getlanguage.c

#sci_gateway/c/sci_getlanguagealias.c \
#sci_gateway/c/sci_getlanguagestable.c \
#sci_gateway/c/sci_addtext.c \
#sci_gateway/c/sci_removetext.c

libscilocalization_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/ 

libscilocalization_la_LDFLAGS = $(JAVA_JNI_LIBS) 

pkglib_LTLIBRARIES = libscilocalization.la

libscilocalization_la_SOURCES = $(LOCALIZATION_C_SOURCES)  $(GATEWAY_C_SOURCES)

libscilocalization_la_LIBADD = 	$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/libs/hashtable/libscihashtable.la

#### Target ######
modulename=localization

#### localization : Conf files ####
libscilocalization_la_rootdir = $(mydatadir)
libscilocalization_la_root_DATA = changelog.txt licence.txt readme.txt version.xml


####localization : init scripts #### 
libscilocalization_la_etcdir = $(mydatadir)/etc
libscilocalization_la_etc_DATA = etc/localization.quit etc/localization.start

#### localization : gateway declaration #### 
libscilocalization_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscilocalization_la_sci_gateway_DATA = sci_gateway/localization_gateway.xml


####localization : include files ####
libscilocalization_la_includedir=$(pkgincludedir)/localization
libscilocalization_la_include_HEADERS = includes/localization.h \
includes/gw_localization.h

if GUI
USEANT=1
endif


include $(top_srcdir)/Makefile.incl.am
