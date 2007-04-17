##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

if JAVA

JVM_C_SOURCES = src/c/JVM_CONSOLE.c \
src/c/JVM_Unix.c \
src/c/jvms.c\
src/c/InitializeJVM.c

libscijvm_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/libs/MALLOC/includes/ -Iincludes/

libscijvm_la_LDFLAGS = $(JAVA_JNI_LIBS) -lpthread -ldl

pkglib_LTLIBRARIES = libscijvm.la

libscijvm_la_SOURCES = $(JVM_C_SOURCES)

libscijvm_la_LIBADD = \
$(top_builddir)/modules/gui/libscigui.la




#### Target ######
target=$(datadir)/modules/jvm


#### jvm : Conf files ####
libscijvm_la_rootdir = $(target)
libscijvm_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml


#### jvm : init scripts #### 
libscijvm_la_etcdir = $(target)/etc
libscijvm_la_etc_DATA = etc/jvm.quit etc/jvm.start

include $(top_srcdir)/Makefile.incl.am

else
		echo "Java disabled. Skip this module."
endif