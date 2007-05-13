##########
### Allan CORNET
### INRIA - Scilab 2007
##########

# List of the c files
CONSOLE_C_SOURCES = src/c/completion.c

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/sci_completion.c \
sci_gateway/c/gw_console.c

# Includes need for the compilation
libsciconsole_la_CFLAGS=  -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/

# Name of the future library (.la will be replace by .so, .a, etc regarding 
# the system)
pkglib_LTLIBRARIES = libsciconsole.la

# All the sources needed by libsciconsole.la
libsciconsole_la_SOURCES = $(CONSOLE_C_SOURCES) $(GATEWAY_C_SOURCES) 

## Libraries necessary
libsciconsole_la_LIBADD = \
	$(top_builddir)/libs/blas/libsciblas.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la

#### Target ######
target=$(datadir)/console

#### console : Conf files ####
libsciconsole_la_rootdir = $(target)
libsciconsole_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml

#### console : init scripts #### 
libsciconsole_la_etcdir = $(target)/etc
libsciconsole_la_etc_DATA = etc/console.quit etc/console.start

#### console : gateway declaration #### 
libsciconsole_la_sci_gatewaydir = $(target)/sci_gateway
libsciconsole_la_sci_gateway_DATA = sci_gateway/console

#### console : include files ####
libsciconsole_la_includedir=$(sciincludedir)/console
libsciconsole_la_include_HEADERS = includes/gw_console.h

# Provides macros compilation, Java compilation, cleaning
include $(top_srcdir)/Makefile.incl.am
