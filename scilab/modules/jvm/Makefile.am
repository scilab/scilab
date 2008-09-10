##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########


libscijvm_la_CFLAGS=  	$(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/libs/dynamiclibrary/includes/ \
			-Iincludes/ \
			-Isrc/c \
			$(XML_FLAGS) \
			-I$(top_srcdir)/modules/output_stream/includes/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			-I$(top_srcdir)/modules/string/includes/ \
			-I$(top_srcdir)/modules/fileio/includes/

pkglib_LTLIBRARIES = libscijvm.la

libscijvm_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(JAVA_JNI_LIBS) -lpthread -ldl $(XML_LIBS)


libscijvm_la_LIBADD =  $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/dynamiclibrary/libscidynamiclibrary.la $(top_builddir)/modules/output_stream/libscioutput_stream.la
modulename=jvm

#### jvm : Conf files ####
libscijvm_la_rootdir = $(mydatadir)
libscijvm_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### jvm : init scripts ####
libscijvm_la_etcdir = $(mydatadir)/etc
libscijvm_la_etc_DATA = etc/jvm.quit etc/jvm.start etc/jvm_options.xml

#### jvm : gateway declaration ####
libscijvm_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscijvm_la_sci_gateway_DATA = sci_gateway/jvm_gateway.xml

#### jvm : include files ####
libscijvm_la_includedir=$(pkgincludedir)/jvm
libscijvm_la_include_HEADERS = includes/TerminateJVM.h \
includes/getScilabJavaVM.h \
includes/getScilabJNIEnv.h \
includes/InitializeJVM.h \
includes/gw_jvm.h \
includes/addToClasspath.h \
includes/addToLibrarypath.h \
includes/catchIfJavaException.h \
includes/getScilabObject.h \
includes/loadBackGroundClassPath.h \
includes/loadOnUseClassPath.h 

if GUI

GATEWAY_C_SOURCES = sci_gateway/c/sci_system_getproperty.c \
sci_gateway/c/sci_system_setproperty.c \
sci_gateway/c/sci_with_embedded_jre.c \
sci_gateway/c/sci_javaclasspath.c \
sci_gateway/c/gw_jvm.c \
sci_gateway/c/sci_javalibrarypath.c


JVM_C_SOURCES = src/c/InitializeJVM.c \
src/c/TerminateJVM.c \
src/c/JVM_Unix.c \
src/c/JVM.c \
src/c/addToClasspath.c \
src/c/loadOnUseClassPath.c \
src/c/loadBackGroundClassPath.c \
src/c/getClasspath.c \
src/c/system_getproperty.c \
src/c/system_setproperty.c \
src/c/loadClasspath.c \
src/c/createMainScilabObject.c \
src/c/JVM_commons.c \
src/c/catchIfJavaException.c \
src/c/loadLibrarypath.c \
src/c/getLibrarypath.c \
src/c/addToLibrarypath.c \
src/c/getJvmOptions.c

USEANT=1

else

JVM_C_SOURCES = src/nojvm/nojvm.c

endif

libscijvm_la_SOURCES = $(JVM_C_SOURCES) $(GATEWAY_C_SOURCES)


include $(top_srcdir)/Makefile.incl.am
