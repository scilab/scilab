##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

SOUND_C_SOURCES = src/raw.c \
src/sox.c \
src/libst.c \
src/wav.c \
src/misc.c

GATEWAY_C_SOURCES = sci_gateway/sci_savewave.c \
sci_gateway/sci_loadwave.c \
sci_gateway/gw_sound.c \
sci_gateway/sci_beep.c \
sci_gateway/sci_PlaySound.c

libsound_la_includedir=$(includedir)/sound

libsound_la_include_HEADERS = includes/sci_PlaySound.h \
includes/sci_beep.h \
includes/sci_savewave.h \
includes/sci_loadwave.h \
includes/gw_sound.h

libsound_la_CFLAGS=  -Iincludes/ -Isrc/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libsound.la

libsound_la_SOURCES = $(SOUND_C_SOURCES) $(GATEWAY_C_SOURCES)

libsound_la_DEPENDENCIES = \
	$(top_builddir)/libs/MALLOC/libmalloc.la \
	$(top_builddir)/modules/core/libcore.la

libsound_la_LIBADD = $(libsound_la_DEPENDENCIES)

libsound_la_PKGCONFIG = sound.pc
