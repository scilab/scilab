# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
# 
# This file is distributed under the same license as the Scilab package.

SOUND_C_SOURCES = src/c/raw.c \
src/c/sox.c \
src/c/wav.c

GATEWAY_C_SOURCES = sci_gateway/c/sci_savewave.c \
sci_gateway/c/sci_loadwave.c \
sci_gateway/c/gw_sound.c \
sci_gateway/c/sci_beep.c \
sci_gateway/c/sci_PlaySound.c

libscisound_la_CFLAGS=  -Iincludes/ \
			-Isrc/c/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes

pkglib_LTLIBRARIES = libscisound.la

libscisound_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscisound_la_SOURCES = $(SOUND_C_SOURCES) $(GATEWAY_C_SOURCES)

libscisound_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la $(top_builddir)/libs/libst/libscilibst.la



#### Target ######
modulename=sound


#### sound : Conf files ####
libscisound_la_rootdir = $(mydatadir)
libscisound_la_root_DATA = changelog.txt license.txt readme.txt version.xml


####�sound : init scripts ####
libscisound_la_etcdir = $(mydatadir)/etc
libscisound_la_etc_DATA = etc/sound.quit etc/sound.start

####�sound : gateway declaration ####
libscisound_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscisound_la_sci_gateway_DATA = sci_gateway/sound_gateway.xml

####�sound : include files ####
libscisound_la_includedir=$(pkgincludedir)/sound
libscisound_la_include_HEADERS = includes/gw_sound.h


####�sound : demos files ####
libscisound_la_demosdir=$(mydatadir)/demos/
libscisound_la_demos_DATA = demos/chimes.wav \
demos/sound.dem.sce

include $(top_srcdir)/Makefile.incl.am
