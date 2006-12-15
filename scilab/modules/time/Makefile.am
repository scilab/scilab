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

libtime_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libtime.la

libtime_la_SOURCES = $(TIME_C_SOURCES) $(GATEWAY_C_SOURCES)

libtime_la_LIBADD = \
       $(top_builddir)/modules/intersci/libintersci.la \
       $(top_builddir)/modules/core/libcore.la \
       $(top_builddir)/libs/MALLOC/libmalloc.la

libtime_la_PKGCONFIG = time.pc


datadir=${prefix}/modules
#### Target ######
target=$(datadir)/time


#### time : Conf files ####
libtime_la_rootdir = $(target)
libtime_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### time : Scilab macros ###
libtime_la_macrodir = $(target)/macros
libtime_la_macro_DATA = macros/etime.sci \
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
libtime_la_etcdir = $(target)/etc
libtime_la_etc_DATA = etc/time.quit etc/time.start

#### time : gateway declaration #### 
libtime_la_sci_gatewaydir = $(target)/sci_gateway
libtime_la_sci_gateway_DATA = sci_gateway/time

#### time : documentation FR #### 
libtime_la_helpfrdir = $(target)/help/fr
libtime_la_helpfr_DATA = help/fr/realtime.htm \
help/fr/calendar.htm \
help/fr/whatis.htm \
help/fr/clock.htm \
help/fr/datenum.htm \
help/fr/datevec.htm \
help/fr/date.htm \
help/fr/eomday.htm \
help/fr/etime.htm \
help/fr/getdate.htm \
help/fr/now.htm \
help/fr/sleep.htm \
help/fr/tic.htm \
help/fr/timer.htm \
help/fr/toc.htm \
help/fr/weekday.htm


#### time : documentation EN #### 
libtime_la_helpendir = $(target)/help/eng
libtime_la_helpen_DATA = help/eng/whatis.htm \
help/eng/calendar.htm \
help/eng/clock.htm \
help/eng/datenum.htm \
help/eng/datevec.htm \
help/eng/date.htm \
help/eng/eomday.htm \
help/eng/etime.htm \
help/eng/getdate.htm \
help/eng/now.htm \
help/eng/realtime.htm \
help/eng/sleep.htm \
help/eng/tic.htm \
help/eng/timer.htm \
help/eng/toc.htm \
help/eng/weekday.htm


#### time : include files ####
libtime_la_includedir=$(includedir)/time
libtime_la_include_HEADERS = includes/gw_time.h \
includes/sci_getdate.h \
includes/sci_timer.h \
includes/sci_realtime.h \
includes/sci_xpause.h \
includes/sci_sleep.h \
includes/sci_calendar.h

