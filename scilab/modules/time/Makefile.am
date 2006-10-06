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


datadir=${prefix}/modules

#### Target ######
target=$(datadir)/time

#### Time : Doc files ####
libtime_la_rootdir = $(target)
libtime_la_root_DATA = changelog.txt \
licence.txt \
readme.txt \
VERSION

#### Time : Scilab macros ###
libtime_la_macrodir = $(target)/macros
libtime_la_macro_DATA = macros/isLeapYear.sci \
macros/etime.sci \
macros/now.sci \
macros/toc.sci \
macros/calendar.sci \
macros/weekday.sci \
macros/clock.sci \
macros/datevec.sci \
macros/date.sci \
macros/datenum.sci \
macros/eomday.sci \
macros/tic.sci \
macros/buildmacros.sce

#### Time : init scripts #### 
libtime_la_etcdir = $(target)/etc
libtime_la_etc_DATA = etc/time.quit \
etc/time.start


#### Time : documentation FR #### 
libtime_la_helpfrdir = $(target)/help/fr
libtime_la_helpfr_DATA = help/fr/timer.xml \
help/fr/etime.xml \
help/fr/now.xml \
help/fr/toc.xml \
help/fr/sleep.xml \
help/fr/calendar.xml \
help/fr/weekday.xml \
help/fr/datevec.xml \
help/fr/date.xml \
help/fr/datenum.xml \
help/fr/getdate.xml \
help/fr/eomday.xml \
help/fr/tic.xml


#### Time : documentation EN #### 
libtime_la_helpendir = $(target)/help/eng
libtime_la_helpen_DATA = help/eng/timer.xml \
help/eng/toc.xml \
help/eng/weekday.xml \
help/eng/clock.xml \
help/eng/datenum.xml \
help/eng/realtime.xml \
help/eng/now.xml \
help/eng/etime.xml \
help/eng/sleep.xml \
help/eng/calendar.xml \
help/eng/datevec.xml \
help/eng/getdate.xml \
help/eng/date.xml \
help/eng/eomday.xml \
help/eng/tic.xml



#### Time : include files ####
libtime_la_includedir=$(includedir)/time
libtime_la_include_HEADERS = includes/sci_getdate.h \
includes/gw_time.h \
includes/sci_realtime.h \
includes/sci_timer.h \
includes/sci_calendar.h \
includes/sci_xpause.h \
includes/sci_sleep.h


libtime_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libtime.la

libtime_la_SOURCES = $(TIME_C_SOURCES) $(GATEWAY_C_SOURCES)

libtime_la_DEPENDENCIES = \
	$(top_builddir)/modules/intersci/libintersci.la \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libtime_la_LIBADD = $(libtime_la_DEPENDENCIES)

libtime_la_PKGCONFIG = time.pc

