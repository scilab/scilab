##########
### Allan CORNET
### INRIA - Scilab 2007
##########

# List of the c files
SHELL_C_SOURCES = src/c/completion.c \
src/c/clc.c \
src/c/tohome.c \
src/c/history.c \
src/c/zzledt_Linux.c 

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/sci_completion.c \
sci_gateway/c/gw_shell.c \
sci_gateway/c/sci_clc.c \
sci_gateway/c/sci_tohome.c \
sci_gateway/c/sci_gethistory.c \
sci_gateway/c/sci_loadhistory.c \
sci_gateway/c/sci_resethistory.c \
sci_gateway/c/sci_savehistory.c

if WITH_JAVA
SHELL_C_SOURCES += src/c/ShellPrintf.c
else
SHELL_C_SOURCES += src/c/ShellPrintf_nojava.c
endif

# Includes need for the compilation
libscishell_la_CFLAGS= $(JAVA_JNI_INCLUDE) -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/

libscishell_la_LDFLAGS = $(JAVA_JNI_LIBS)

# Name of the future library (.la will be replace by .so, .a, etc regarding 
# the system)
pkglib_LTLIBRARIES = libscishell.la

# All the sources needed by libscishell.la
libscishell_la_SOURCES = $(SHELL_C_SOURCES) $(GATEWAY_C_SOURCES) 

## Libraries necessary
libscishell_la_LIBADD = \
	$(top_builddir)/libs/blas/libsciblas.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la


modulename=shell

#### shell : Conf files ####
libscishell_la_rootdir = $(mydatadir)
libscishell_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

#### shell : init scripts #### 
libscishell_la_etcdir = $(mydatadir)/etc
libscishell_la_etc_DATA = etc/shell.quit etc/shell.start

#### shell : gateway declaration #### 
libscishell_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscishell_la_sci_gateway_DATA = sci_gateway/shell_gateway.xml

#### shell : include files ####
libscishell_la_includedir=$(pkgincludedir)/shell
libscishell_la_include_HEADERS = includes/gw_shell.h

if WITH_JAVA

USEANT=1

endif

# Provides macros compilation, Java compilation, cleaning
include $(top_srcdir)/Makefile.incl.am
