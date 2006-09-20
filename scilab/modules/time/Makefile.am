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

libtime_la_includedir=$(includedir)/time

libtime_la_include_HEADERS = includes/sci_getdate.h \
includes/gw_time.h \
includes/sci_realtime.h \
includes/sci_timer.h \
includes/sci_calendar.h \
includes/sci_xpause.h \
includes/sci_sleep.h

libtime_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = libtime.la

libtime_la_SOURCES = $(TIME_C_SOURCES) $(GATEWAY_C_SOURCES)

libtime_la_DEPENDENCIES = \
	$(top_builddir)/modules/intersci/libintersci.la \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la




libtime_la_LIBADD = $(libtime_la_DEPENDENCIES)

libtime_la_PKGCONFIG = time.pc
