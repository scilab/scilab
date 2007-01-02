##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

LOCALIZATION_C_SOURCES = src/hashtable_localization.c \
src/localization.c

liblocalization_la_CFLAGS=  -Iincludes/ -Isrc/ -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/libs/hashtable/ $(XML_FLAGS)
liblocalization_la_LDFLAGS= $(XML_LIBS)

lib_LTLIBRARIES = liblocalization.la

liblocalization_la_SOURCES = $(LOCALIZATION_C_SOURCES) 

liblocalization_la_LIBADD = 	$(top_builddir)/libs/MALLOC/libmalloc.la \
$(top_builddir)/libs/hashtable/libhashtable.la

liblocalization_la_PKGCONFIG = localization.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/localization


#### localization : Conf files ####
liblocalization_la_rootdir = $(target)
liblocalization_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### localization : Scilab macros ###
liblocalization_la_macrodir = $(target)/macros
liblocalization_la_macro_DATA = macros/setlanguage.sci macros/setlanguage.bin macros/names macros/lib macros/buildmacros.sce


#### localization : init scripts #### 
liblocalization_la_etcdir = $(target)/etc
liblocalization_la_etc_DATA = etc/localization.quit etc/localization.start


#### localization : include files ####
liblocalization_la_includedir=$(includedir)/localization
liblocalization_la_include_HEADERS = includes/localization.h

include $(top_srcdir)/Makefile.incl.am
