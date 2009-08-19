##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### DIGITEO - Scilab 2009
##########

### SOURCES ###
CALLSCILAB_C_SOURCES = src/c/CallScilab.c \
		src/c/fromc.c \
		src/c/fromjava.c \
		src/c/SendScilabJobs.c

GATEWAY_C_SOURCES = sci_gateway/c/sci_fromjava.c \
		sci_gateway/c/sci_fromc.c \
		sci_gateway/c/gw_call_scilab.c

libscicall_scilab_la_CFLAGS=  	-I$(srcdir)/includes/ \
			-I$(srcdir)/src/c/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/libs/doublylinkedlist/includes \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/action_binding/includes \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/fileio/includes \
			-I$(top_srcdir)/modules/graphics/includes \
			-I$(top_srcdir)/modules/operations/includes \
			-I$(top_srcdir)/modules/core/src/c/

pkglib_LTLIBRARIES = libscicall_scilab.la

libscicall_scilab_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscicall_scilab_la_SOURCES = $(CALLSCILAB_C_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(libscicall_scilab_la_SOURCES)
INCLUDE_FLAGS = $(libscicall_scilab_la_CFLAGS)

libscicall_scilab_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la




#### Target ######
modulename=call_scilab


#### call_scilab : Conf files ####
libscicall_scilab_la_rootdir = $(mydatadir)
libscicall_scilab_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### call_scilab : init scripts ####
libscicall_scilab_la_etcdir = $(mydatadir)/etc
libscicall_scilab_la_etc_DATA = etc/call_scilab.quit etc/call_scilab.start

#### call_scilab : gateway declaration ####
libscicall_scilab_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscicall_scilab_la_sci_gateway_DATA = sci_gateway/call_scilab_gateway.xml

#### call_scilab : include files ####
libscicall_scilab_la_includedir=$(pkgincludedir)/call_scilab
libscicall_scilab_la_include_HEADERS = includes/CallScilab.h \
	includes/fromjava.h

include $(top_srcdir)/Makefile.incl.am
