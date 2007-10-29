##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########




#### Target ######
modulename=javasci


if JAVASCI
### All the compilation process of java is deleguated to ant

#### Javasci : sources ####
JAVASCI_C_SOURCES = src/c/javasci_Scilab2.c \
src/c/javasci_globals.c \
src/c/javasci_SciAbstractArray.c \
src/c/javasci_SciStringArray.c \
src/c/javasci_SciComplexArray.c \
src/c/fix.c \
src/c/javasci_SciDoubleArray.c \
src/c/javasci_Scilab.c \
src/c/javasci_SciBooleanArray.c

pkglib_LTLIBRARIES = libjavasci.la

libjavasci_la_SOURCES = $(JAVASCI_C_SOURCES)

libjavasci_la_CFLAGS = 	-Iincludes/ \
			$(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/libs/doublylinkedlist/includes \
			-I$(top_srcdir)/modules/output_stream/includes

libjavasci_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
	$(top_builddir)/modules/libscilab.la

#### javasci : Conf files ####
libjavasci_la_rootdir = $(mydatadir)
libjavasci_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

#### javasci : init scripts ####
libjavasci_la_etcdir = $(mydatadir)/etc
libjavasci_la_etc_DATA = etc/javasci.quit etc/javasci.start


#### javasci : include files ####
libjavasci_la_includedir=$(pkgincludedir)/javasci
libjavasci_la_include_HEADERS = includes/javasci_Scilab.h \
includes/javasci_Scilab2.h \
includes/javasci_globals.h \
includes/javasci_SciStringArray.h \
includes/javasci_SciComplexArray.h \
includes/javasci_SciDoubleArray.h \
includes/javasci_SciBooleanArray.h \
includes/javasci_SciAbstractArray.h

USEANT=1



include $(top_srcdir)/Makefile.incl.am

endif