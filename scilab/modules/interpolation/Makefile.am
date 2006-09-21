INTERPOLATION_C_SOURCES = src/c/interpolation.c \
src/c/someinterp.c

GATEWAY_C_SOURCES = sci_gateway/c/sci_lsq_splin.c \
sci_gateway/c/sci_interp.c \
sci_gateway/c/sci_interp2d.c \
sci_gateway/c/sci_splin.c \
sci_gateway/c/gw_interpolation.c \
sci_gateway/c/sci_cshep2d.c \
sci_gateway/c/sci_interp3d.c \
sci_gateway/c/sci_linear_interpn.c \
sci_gateway/c/sci_splin2d.c \
sci_gateway/c/sci_splin3d.c \
sci_gateway/c/sci_bsplin3val.c \
sci_gateway/c/sci_eval_cshep2d.c

libinterpolation_la_includedir=$(includedir)/interpolation

libinterpolation_la_include_HEADERS = includes/gw_interpolation.h \
includes/interpolation.h

libinterpolation_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/
#-Isrc/c/
lib_LTLIBRARIES = libinterpolation.la

libinterpolation_la_SOURCES = $(INTERPOLATION_C_SOURCES) $(GATEWAY_C_SOURCES) 

libinterpolation_la_DEPENDENCIES = \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la 



libinterpolation_la_LIBADD = $(libinterpolation_la_DEPENDENCIES)

libinterpolation_la_PKGCONFIG = interpolation.pc
