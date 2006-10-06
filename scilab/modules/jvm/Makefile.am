##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

JVM_C_SOURCES = src/JVM_CONSOLE.c \
src/JVM_Unix.c \
src/jvms.c

libscijvm_la_includedir=$(includedir)/scijvm

libscijvm_la_include_HEADERS = 

libscijvm_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/libs/MALLOC/includes/

libscijvm_la_LDFLAGS = $(JAVA_JNI_LIBS) -lpthread -ldl

lib_LTLIBRARIES = libscijvm.la

libscijvm_la_SOURCES = $(JVM_C_SOURCES)

libscijvm_la_LIBADD = \
$(top_builddir)/modules/gui/libgui.la

libscijvm_la_PKGCONFIG = scijvm.pc
