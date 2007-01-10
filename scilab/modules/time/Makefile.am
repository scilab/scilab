##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

###  SOURCES ###
TIME_C_SOURCES = src/realtime.c \
src/timer.c \
src/getdate.c

GATEWAY_C_SOURCES = sci_gateway/sci_realtime.c \
sci_gateway/sci_timer.c \
sci_gateway/sci_calendar.c \
sci_gateway/sci_xpause.c \
sci_gateway/sci_sleep.c \
sci_gateway/sci_getdate.c \
sci_gateway/gw_time.c

libscitime_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/ -Isrc/

pkglib_LTLIBRARIES = libscitime.la

libscitime_la_SOURCES = $(TIME_C_SOURCES) $(GATEWAY_C_SOURCES)

libscitime_la_LIBADD = \
       $(top_builddir)/modules/intersci/libsciintersci.la \
       $(top_builddir)/modules/core/libscicore.la \
       $(top_builddir)/libs/MALLOC/libscimalloc.la

libscitime_la_PKGCONFIG = time.pc


datadir=${prefix}/modules
#### Target ######
target=$(datadir)/time


#### time : Conf files ####
libscitime_la_rootdir = $(target)
libscitime_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### time : Scilab macros ###
libscitime_la_macrodir = $(target)/macros
libscitime_la_macro_DATA22 = macros/etime.sci \
macros/now.sci \
macros/toc.sci \
macros/calendar.sci \
macros/weekday.sci \
macros/clock.sci \
macros/datevec.sci \
macros/date.sci \
macros/datenum.sci \
macros/eomday.sci \
macros/isLeapYear.sci \
macros/tic.sci macros/calendar.bin \
macros/clock.bin \
macros/datenum.bin \
macros/date.bin \
macros/datevec.bin \
macros/eomday.bin \
macros/etime.bin \
macros/isLeapYear.bin \
macros/now.bin \
macros/tic.bin \
macros/toc.bin \
macros/weekday.bin macros/names macros/lib macros/buildmacros.sce


#### time : init scripts #### 
libscitime_la_etcdir = $(target)/etc
libscitime_la_etc_DATA = etc/time.quit etc/time.start

#### time : gateway declaration #### 
libscitime_la_sci_gatewaydir = $(target)/sci_gateway
libscitime_la_sci_gateway_DATA = sci_gateway/time

#### time : include files ####
libscitime_la_includedir=$(includedir)/time
libscitime_la_include_HEADERS = includes/gw_time.h \
includes/sci_getdate.h \
includes/sci_timer.h \
includes/sci_realtime.h \
includes/sci_xpause.h \
includes/sci_sleep.h \
includes/sci_calendar.h

include $(top_srcdir)/Makefile.incl.am
