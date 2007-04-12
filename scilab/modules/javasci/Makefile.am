##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

include $(top_srcdir)/Makefile.incl.am

#### Target ######
target=$(datadir)/javasci

#### Java sources ####
JAVAFILES = src/java/javasci/Scilab.java \
	src/java/javasci/BadDataArgumentException.java \
	src/java/javasci/SciAbstractArray.java \
	src/java/javasci/SciAbstractDataType.java \
	src/java/javasci/SciDoubleArray.java \
	src/java/javasci/SciBooleanArray.java \
	src/java/javasci/SciDouble.java \
	src/java/javasci/SciBoolean.java \
	src/java/javasci/SciComplexArray.java \
	src/java/javasci/SciStringArray.java \
	src/java/javasci/SciComplex.java \
	src/java/javasci/SciString.java

#### Where are the sources (mandatory) ####
CLASSPATH = src/java/

#### javasci : jar file #### 
JARFILE = javasci.jar

BUILT_SOURCES+=$(JARFILE)

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

libjavasci_la_CFLAGS = -Iincludes/ $(JAVA_JNI_INCLUDE)

libjavasci_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
	$(top_builddir)/modules/libscilab.la

#### javasci : Conf files ####
libjavasci_la_rootdir = $(target)
libjavasci_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml

#### javasci : init scripts #### 
libjavasci_la_etcdir = $(target)/etc
libjavasci_la_etc_DATA = etc/javasci.quit etc/javasci.start


#### javasci : include files ####
libjavasci_la_includedir=$(sciincludedir)/javasci
libjavasci_la_include_HEADERS = includes/javasci_Scilab.h \
includes/javasci_Scilab2.h \
includes/javasci_globals.h \
includes/javasci_SciStringArray.h \
includes/javasci_SciComplexArray.h \
includes/javasci_SciDoubleArray.h \
includes/javasci_SciBooleanArray.h \
includes/javasci_SciAbstractArray.h


tests:: $(JARFILE) $(JAVAFILES)
	@(cd tests; make tests)

install-data-local: $(JARFILE)
	$(INSTALL_DATA) $(JARFILE) $(DESTDIR)$(pkglibdir)/
