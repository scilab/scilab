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

#### sound : documentation FR #### 
libsound_la_helpfrdir = $(target)/help/fr
libsound_la_helpfr_DATA2 = help/fr/loadwave.htm \
help/fr/auwrite.htm \
help/fr/lin2mu.htm \
help/fr/savewave.htm \
help/fr/mu2lin.htm \
help/fr/playsnd.htm \
help/fr/soundsec.htm \
help/fr/sound.htm \
help/fr/wavread.htm \
help/fr/wavwrite.htm \
help/fr/beep.htm \
help/fr/whatis.htm \
help/fr/auread.htm \
help/fr/analyze.htm \
help/fr/mapsound.htm


#### sound : documentation EN #### 
libsound_la_helpendir = $(target)/help/eng
libsound_la_helpen_DATA2 = help/eng/whatis.htm \
help/eng/loadwave.htm \
help/eng/analyze.htm \
help/eng/auread.htm \
help/eng/auwrite.htm \
help/eng/beep.htm \
help/eng/lin2mu.htm \
help/eng/mapsound.htm \
help/eng/mu2lin.htm \
help/eng/playsnd.htm \
help/eng/savewave.htm \
help/eng/soundsec.htm \
help/eng/sound.htm \
help/eng/wavread.htm \
help/eng/wavwrite.htm


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
