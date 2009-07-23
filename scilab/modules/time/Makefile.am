##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

### SOURCES ###
TIME_C_SOURCES = src/c/realtime.c \
src/c/timer.c \
src/c/getdate.c 

GATEWAY_C_SOURCES = sci_gateway/c/sci_realtime.c \
sci_gateway/c/sci_timer.c \
sci_gateway/c/sci_calendar.c \
sci_gateway/c/sci_xpause.c \
sci_gateway/c/sci_sleep.c \
sci_gateway/c/sci_getdate.c \
sci_gateway/c/gw_time.c

libscitime_la_CFLAGS=  	-I$(srcdir)/includes/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(srcdir)/src/c/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/operations/includes \
			-I$(top_srcdir)/modules/api_scilab/includes

pkglib_LTLIBRARIES = libscitime.la

libscitime_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscitime_la_SOURCES = $(TIME_C_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(libscitime_la_SOURCES)
INCLUDE_FLAGS = $(libscitime_la_CFLAGS)

libscitime_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la




#### Target ######
modulename=time


#### time : Conf files ####
libscitime_la_rootdir = $(mydatadir)
libscitime_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### time : init scripts ####
libscitime_la_etcdir = $(mydatadir)/etc
libscitime_la_etc_DATA = etc/time.quit etc/time.start

#### time : gateway declaration ####
libscitime_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscitime_la_sci_gateway_DATA = sci_gateway/time_gateway.xml

#### time : include files ####
libscitime_la_includedir=$(pkgincludedir)/time
libscitime_la_include_HEADERS = includes/gw_time.h \
includes/dynlib_time.h \
includes/realtime.h \
includes/timer.h


include $(top_srcdir)/Makefile.incl.am
