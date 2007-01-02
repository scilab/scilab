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


libsound_la_CFLAGS=  -Iincludes/ -Isrc/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libsound.la

libsound_la_SOURCES = $(SOUND_C_SOURCES) $(GATEWAY_C_SOURCES)

libsound_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libmalloc.la \
	$(top_builddir)/modules/core/libcore.la

libsound_la_PKGCONFIG = sound.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/sound


#### sound : Conf files ####
libsound_la_rootdir = $(target)
libsound_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### sound : Scilab macros ###
libsound_la_macrodir = $(target)/macros
libsound_la_macro_DATA = macros/analyze.sci \
macros/soundsec.sci \
macros/lin2mu.sci \
macros/mapsound.sci \
macros/mu2lin.sci \
macros/sound.sci \
macros/wavwrite.sci \
macros/wavread.sci \
macros/playsnd.sci \
macros/auwrite.sci \
macros/auread.sci macros/analyze.bin \
macros/auread.bin \
macros/auwrite.bin \
macros/lin2mu.bin \
macros/mapsound.bin \
macros/mu2lin.bin \
macros/playsnd.bin \
macros/sound.bin \
macros/soundsec.bin \
macros/wavread.bin \
macros/wavwrite.bin macros/names macros/lib macros/buildmacros.sce


#### sound : init scripts #### 
libsound_la_etcdir = $(target)/etc
libsound_la_etc_DATA = etc/sound.quit etc/sound.start

#### sound : gateway declaration #### 
libsound_la_sci_gatewaydir = $(target)/sci_gateway
libsound_la_sci_gateway_DATA = sci_gateway/sound

#### sound : include files ####
libsound_la_includedir=$(includedir)/sound
libsound_la_include_HEADERS = includes/sci_beep.h \
includes/sci_PlaySound.h \
includes/sci_savewave.h \
includes/sci_loadwave.h \
includes/gw_sound.h


#### sound : demos files ####
libsound_la_demosdir=$(target)/demos/
libsound_la_demos_DATA = demos/chimes.wav \
demos/sound.dem

include $(top_srcdir)/Makefile.incl.am
