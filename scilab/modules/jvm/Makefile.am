##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

JVM_C_SOURCES = src/c/InitializeJVM.c \
src/c/TerminateJVM.c \
src/c/JVM_Unix.c \
src/c/JVM.c

GATEWAY_C_SOURCES = sci_gateway/c/gw_jvm.c \
sci_gateway/c/sci_jvm_version.c \
sci_gateway/c/sci_with_embedded_jre.c

libscijvm_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/libs/MALLOC/includes/ -Iincludes/

libscijvm_la_LDFLAGS = $(JAVA_JNI_LIBS) -lpthread -ldl

pkglib_LTLIBRARIES = libscijvm.la

libscijvm_la_SOURCES = $(JVM_C_SOURCES) $(GATEWAY_C_SOURCES)

#### Target ######
modulename=jvm

#### jvm : Conf files ####
libscijvm_la_rootdir = $(mydatadir)
libscijvm_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml

#### jvm : init scripts #### 
libscijvm_la_etcdir = $(mydatadir)/etc
libscijvm_la_etc_DATA = etc/jvm.quit etc/jvm.start

#### jvm : gateway declaration #### 
libscijvm_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscijvm_la_sci_gateway_DATA = sci_gateway/jvm_gateway.xml

#### jvm : include files ####
libscijvm_la_includedir=$(pkgincludedir)/jvm
libscijvm_la_include_HEADERS = includes/TerminateJVM.h \
includes/getScilabJVM.h \
includes/InitializeJVM.h \
includes/gw_jvm.h

include $(top_srcdir)/Makefile.incl.am
