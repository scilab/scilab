##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

if JAVA

JVM_C_SOURCES = src/c/InitializeJVM.c \
src/c/TerminateJVM.c \
src/c/JVM_Unix.c \
src/c/JVM.c

libscijvm_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/libs/MALLOC/includes/ -Iincludes/

libscijvm_la_LDFLAGS = $(JAVA_JNI_LIBS) -lpthread -ldl

pkglib_LTLIBRARIES = libscijvm.la

libscijvm_la_SOURCES = $(JVM_C_SOURCES)

libscijvm_la_LIBADD = \
$(top_builddir)/modules/gui/libscigui.la

modulename=jvm


#### jvm : Conf files ####
libscijvm_la_rootdir = $(mydatadir)
libscijvm_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml


#### jvm : init scripts #### 
libscijvm_la_etcdir = $(mydatadir)/etc
libscijvm_la_etc_DATA = etc/jvm.quit etc/jvm.start

#### gui : include files ####
libscijvm_la_includedir=$(pkgincludedir)/jvm
libscijvm_la_include_HEADERS = includes/exportimport.h \
includes/InitializeJVM.h \
includes/JVM_CONSOLE.h


include $(top_srcdir)/Makefile.incl.am

else
		echo "Java disabled. Skip this module."
endif
