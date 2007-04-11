##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

SOUND_C_SOURCES = src/c/raw.c \
src/c/sox.c \
src/c/wav.c

GATEWAY_C_SOURCES = sci_gateway/c/sci_savewave.c \
sci_gateway/c/sci_loadwave.c \
sci_gateway/c/gw_sound.c \
sci_gateway/c/sci_beep.c \
sci_gateway/c/sci_PlaySound.c


libscisound_la_CFLAGS=  -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/

pkglib_LTLIBRARIES = libscisound.la

libscisound_la_SOURCES = $(SOUND_C_SOURCES) $(GATEWAY_C_SOURCES)

libscisound_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
    $(top_builddir)/libs/libst/libscilibst.la \
	$(top_builddir)/modules/core/libscicore.la




#### Target ######
target=$(datadir)/sound


#### sound : Conf files ####
libscisound_la_rootdir = $(target)
libscisound_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### sound : init scripts #### 
libscisound_la_etcdir = $(target)/etc
libscisound_la_etc_DATA = etc/sound.quit etc/sound.start

#### sound : gateway declaration #### 
libscisound_la_sci_gatewaydir = $(target)/sci_gateway
libscisound_la_sci_gateway_DATA = sci_gateway/sound_gateway.xml

#### sound : include files ####
libscisound_la_includedir=$(sciincludedir)/sound
libscisound_la_include_HEADERS = includes/gw_sound.h


#### sound : demos files ####
libscisound_la_demosdir=$(target)/demos/
libscisound_la_demos_DATA = demos/chimes.wav \
demos/sound.dem

include $(top_srcdir)/Makefile.incl.am
