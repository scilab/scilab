##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########



#### Target ######
modulename=javasci

#### Java sources ####
#JAVAFILES = src/java/javasci/javasci/Scilab.java \
#	src/java/javasci/javasci/BadDataArgumentException.java \
#	src/java/javasci/javasci/SciAbstractArray.java \
#	src/java/javasci/javasci/SciAbstractDataType.java \
#	src/java/javasci/javasci/SciDoubleArray.java \
#	src/java/javasci/javasci/SciBooleanArray.java \
#	src/java/javasci/javasci/SciDouble.java \
#	src/java/javasci/javasci/SciBoolean.java \
#	src/java/javasci/javasci/SciComplexArray.java \
#	src/java/javasci/javasci/SciStringArray.java \
#	src/java/javasci/javasci/SciComplex.java \
#	src/java/javasci/javasci/SciString.java \
#	src/java/javasci/javasci/ClassPath.java

#### Where are the sources (mandatory) ####
#CLASSPATH = src/java/javasci/

#### javasci : jar file #### 
#JARFILE = javasci.jar

#BUILT_SOURCES+=$(JARFILE)

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

tests:: $(JARFILE) $(JAVAFILES)
	@(cd tests; make tests)

include $(top_srcdir)/Makefile.incl.am
