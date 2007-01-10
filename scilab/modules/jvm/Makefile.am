##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

if JAVA

JVM_C_SOURCES = src/JVM_CONSOLE.c \
src/JVM_Unix.c \
src/jvms.c

libsciscijvm_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/libs/MALLOC/includes/

libsciscijvm_la_LDFLAGS = $(JAVA_JNI_LIBS) -lpthread -ldl

pkglib_LTLIBRARIES = libsciscijvm.la

libsciscijvm_la_SOURCES = $(JVM_C_SOURCES)

libsciscijvm_la_LIBADD = \
$(top_builddir)/modules/gui/libscigui.la

libsciscijvm_la_PKGCONFIG = scijvm.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/jvm


#### jvm : Conf files ####
libscijvm_la_rootdir = $(target)
libscijvm_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### jvm : Scilab macros ###
libscijvm_la_macrodir = $(target)/macros
libscijvm_la_macro_DATA22 =  macros/buildmacros.sce


#### jvm : init scripts #### 
libscijvm_la_etcdir = $(target)/etc
libscijvm_la_etc_DATA = etc/jvm.quit etc/jvm.start

include $(top_srcdir)/Makefile.incl.am

else
		echo "Java disabled. Skip this module."
endif