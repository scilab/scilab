# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
#
# This file is distributed under the same license as the Scilab package.


GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_beep.c \
    sci_gateway/c/sci_PlaySound.c

libscisound_la_CPPFLAGS= \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/fileio/includes \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/string/includes \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscisound.la


libscisound_la_SOURCES = $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(libscisound_la_SOURCES)
INCLUDE_FLAGS = $(libscisound_la_CPPFLAGS)


#### Target ######
modulename=sound


#### sound : Conf files ####
libscisound_la_rootdir = $(mydatadir)
libscisound_la_root_DATA =  license.txt


####�sound : init scripts ####
libscisound_la_etcdir = $(mydatadir)/etc
libscisound_la_etc_DATA = etc/sound.quit etc/sound.start

####�sound : gateway declaration ####
libscisound_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscisound_la_sci_gateway_DATA = sci_gateway/sound_gateway.xml

####�sound : include files ####
libscisound_la_includedir=$(pkgincludedir)

include $(top_srcdir)/Makefile.incl.am
