##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

if JAVA

JVM_C_SOURCES = src/JVM_CONSOLE.c \
src/JVM_Unix.c \
src/jvms.c

libscijvm_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/libs/MALLOC/includes/

libscijvm_la_LDFLAGS = $(JAVA_JNI_LIBS) -lpthread -ldl

lib_LTLIBRARIES = libscijvm.la

libscijvm_la_SOURCES = $(JVM_C_SOURCES)

libscijvm_la_LIBADD = \
$(top_builddir)/modules/gui/libgui.la

libscijvm_la_PKGCONFIG = scijvm.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/jvm


#### jvm : Conf files ####
libjvm_la_rootdir = $(target)
libjvm_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### jvm : Scilab macros ###
libjvm_la_macrodir = $(target)/macros
libjvm_la_macro_DATA =  macros/buildmacros.sce


#### jvm : init scripts #### 
libjvm_la_etcdir = $(target)/etc
libjvm_la_etc_DATA = etc/jvm.quit etc/jvm.start

include $(top_srcdir)/Makefile.incl.am

else
		echo "Java disabled. Skip this module."
endif