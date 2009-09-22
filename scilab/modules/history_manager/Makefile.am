##########
### Allan CORNET
### INRIA - Scilab 2007
########## *


# List of the cpp files
HISTORY_MANAGER_CPP_SOURCES = src/cpp/CommandLine.cpp \
src/cpp/HistoryFile.cpp \
src/cpp/HistoryManager.cpp \
src/cpp/HistorySearch.cpp

# List of the c files
HISTORY_MANAGER_C_SOURCES = src/c/getCommentDateSession.c

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/gw_history_manager.c \
sci_gateway/c/sci_addhistory.c  \
sci_gateway/c/sci_displayhistory.c  \
sci_gateway/c/sci_gethistory.c  \
sci_gateway/c/sci_gethistoryfile.c  \
sci_gateway/c/sci_historysize.c  \
sci_gateway/c/sci_loadhistory.c  \
sci_gateway/c/sci_removelinehistory.c  \
sci_gateway/c/sci_resethistory.c  \
sci_gateway/c/sci_saveafterncommands.c  \
sci_gateway/c/sci_saveconsecutivecommands.c  \
sci_gateway/c/sci_savehistory.c  \
sci_gateway/c/sci_sethistoryfile.c  \
sci_gateway/c/sci_historymanager.c

if GUI

# List of the c files
HISTORY_MANAGER_C_SOURCES += src/jni/HistoryManagement_wrap.c

USEANT=1

endif



# Includes need for the compilation
libscihistory_manager_la_CPPFLAGS =  $(JAVA_JNI_INCLUDE) -I$(srcdir)/includes/ -I$(srcdir)/src/cpp/ -I$(srcdir)/src/c/ -I$(top_srcdir)/modules/api_scilab/includes -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/modules/output_stream/includes/ -I$(top_srcdir)/modules/fileio/includes/

# Includes need for the compilation
libscihistory_manager_la_CFLAGS= 	$(JAVA_JNI_INCLUDE) \
-I$(srcdir)/includes/ \
-I$(srcdir)/src/c/ \
-I$(top_srcdir)/libs/MALLOC/includes/ \
-I$(top_srcdir)/modules/console/includes/ \
-I$(top_srcdir)/modules/history_manager/includes/ \
-I$(top_srcdir)/modules/output_stream/includes/ \
-I$(top_srcdir)/modules/api_scilab/includes \
-I$(top_srcdir)/modules/fileio/includes/

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscihistory_manager.la

libscihistory_manager_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

# All the sources needed by libscihistory_manager.la
libscihistory_manager_la_SOURCES = $(HISTORY_MANAGER_C_SOURCES) $(HISTORY_MANAGER_CPP_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(HISTORY_MANAGER_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscihistory_manager_la_CFLAGS)

## Libraries necessary
libscihistory_manager_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la $(top_builddir)/modules/fileio/libscifileio.la
modulename=history_manager

#### history_manager : Conf files ####
libscihistory_manager_la_rootdir = $(mydatadir)
libscihistory_manager_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### history_manager : init scripts ####
libscihistory_manager_la_etcdir = $(mydatadir)/etc
libscihistory_manager_la_etc_DATA = etc/history_manager.quit etc/history_manager.start

#### history_manager : gateway declaration ####
libscihistory_manager_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihistory_manager_la_sci_gateway_DATA = sci_gateway/history_manager_gateway.xml

#### history_manager : include files ####
libscihistory_manager_la_includedir=$(pkgincludedir)
libscihistory_manager_la_include_HEADERS = includes/gw_history_manager.h \
includes/HistoryManager.h \
includes/InitializeHistoryManager.h \
includes/TerminateHistoryManager.h \
includes/dynlib_history_manager.h

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/HistoryManagement.i

if SWIG
BUILT_SOURCES = swig
endif

# Provides macros compilation, Java compilation, cleaning
include $(top_srcdir)/Makefile.incl.am
