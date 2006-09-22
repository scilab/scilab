##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

JVM_C_SOURCES = src/JVM_CONSOLE.c \
src/JVM_Unix.c \
src/jvms.c

libjvm_la_includedir=$(includedir)/jvm

libjvm_la_include_HEADERS = 

libjvm_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/libs/MALLOC/includes/

libjvm_la_LDFLAGS = $(JAVA_JNI_LIBS) -lpthread -ldl

lib_LTLIBRARIES = libjvm.la

libjvm_la_SOURCES = $(JVM_C_SOURCES)

libjvm_la_DEPENDENCIES =
#	$(top_builddir)/modules/gui/libgui.la

libjvm_la_LIBADD = $(libjvm_la_DEPENDENCIES)

libjvm_la_PKGCONFIG = jvm.pc
